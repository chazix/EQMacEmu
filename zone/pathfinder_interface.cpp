#include "../common/seperator.h"
#include "client.h"
#include "pathfinder_null.h"
#include "pathfinder_nav_mesh.h"
#include "pathfinder_waypoint.h"
#include "../common/global_define.h"
#include "../common/eqemu_logsys.h"
#include <sys/stat.h>

IPathfinder *IPathfinder::Load(const std::string &zone) {
	struct stat statbuffer;

	// this always defaults to: "Maps/" if not specified in the config file
	std::string maps_dir = Config->MapDir;
	if (maps_dir[maps_dir.length() - 1] != '/') {
		maps_dir += "/";
	}

	std::string navmesh_path = StringFormat("%s%s.nav", maps_dir, zone.c_str());
	if (stat(navmesh_path.c_str(), &statbuffer) == 0) {
		return new PathfinderNavmesh(navmesh_path);
	}

	std::string old_navmesh_path = StringFormat("%s%s.path", maps_dir, zone.c_str());
	if (stat(old_navmesh_path.c_str(), &statbuffer) == 0) {
		return new PathfinderWaypoint(old_navmesh_path);
	}
	
	return new PathfinderNull();
}