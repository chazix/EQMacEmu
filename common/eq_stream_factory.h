#ifndef _EQSTREAMFACTORY_H

#define _EQSTREAMFACTORY_H

#include <memory>
#include <queue>
#include <map>

#include "../common/eq_stream.h"
#include "../common/condition.h"
#include "../common/timeoutmgr.h"

class EQStream;
class Timer;

using EQStreamIterator = std::map<std::pair<uint32, uint16>, std::shared_ptr<EQStream>>::iterator;
using EQOldStreamIterator = std::map<std::pair<uint32, uint16>, std::shared_ptr<EQOldStream>>::iterator;

class RecvBuffer {
	private:
		bool isnew;
		uint32 length;
		std::unique_ptr<unsigned char[]> buffer;
		std::pair<unsigned long, unsigned short> streamkey;
		sockaddr_in from;

	public:
		RecvBuffer(bool isnew, uint32 len, const unsigned char* buf, std::pair<unsigned long, unsigned short> key, sockaddr_in& f) : isnew(isnew), length(len), streamkey(key), from(f) {
			buffer.reset(new unsigned char[len]);
			memcpy(buffer.get(), buf, len);
		}

		bool IsNew() const { return isnew; }
		unsigned char* Buffer() const { return buffer.get(); }
		uint32 Length() const { return length; }
		const std::pair<unsigned long, unsigned short>& StreamKey() const { return streamkey; }
		const sockaddr_in& From() const { return from; }
};

class EQStreamFactory : private Timeoutable {
	private:
		int sock;
		int Port;

		bool ReaderRunning;
		Mutex MReaderRunning;
		bool WriterRunning;
		Mutex MWriterRunning;

		Condition WriterWork;

		EQStreamType StreamType;

		std::queue<std::shared_ptr<EQStream>> NewStreams;
		Mutex MNewStreams;

		std::map<std::pair<uint32, uint16>, std::shared_ptr<EQStream>> Streams;
		Mutex MStreams;

		std::queue<std::shared_ptr<EQOldStream>> NewOldStreams;

		std::map<std::pair<uint32, uint16>, std::shared_ptr<EQOldStream>> OldStreams;

		virtual void CheckTimeout();

		Timer *DecayTimer;

		uint32 stream_timeout;

	public:
		EQStreamFactory(EQStreamType type, uint32 timeout = 61000) : Timeoutable(5000), stream_timeout(timeout) { ReaderRunning=false; WriterRunning=false; StreamType=type; sock=-1; }
		EQStreamFactory(EQStreamType type, int port, uint32 timeout = 61000);

		std::shared_ptr<EQStream> Pop();
		void Push(std::shared_ptr<EQStream> s);

		std::shared_ptr<EQOldStream> PopOld();
		void PushOld(std::shared_ptr<EQOldStream> s);

		bool Open();
		bool Open(unsigned long port) { Port=port; return Open(); }
		bool IsOpen() { return sock!=-1; }
		void Close();
		void ReaderLoop();
		void ProcessLoopNew(const RecvBuffer& recvBuffer, EQStreamIterator iterator);
		void ProcessLoopOld(const RecvBuffer& recvBuffer, EQOldStreamIterator iterator);
		void WriterLoop();
		void Stop() { StopReader(); StopWriter(); }
		void StopReader() { MReaderRunning.lock(); ReaderRunning=false; MReaderRunning.unlock(); }
		void StopWriter() { MWriterRunning.lock(); WriterRunning=false; MWriterRunning.unlock(); WriterWork.Signal(); }
		void SignalWriter() { WriterWork.Signal(); }
};

#endif
