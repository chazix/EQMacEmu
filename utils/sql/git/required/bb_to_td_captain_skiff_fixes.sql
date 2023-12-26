INSERT INTO `eqemu`.`grid_entries` (`gridid`, `zoneid`, `number`, `x`, `y`, `z`, `heading`, `pause`) VALUES ('16', '68', '0', '3202', '855', '-13', '131', '0');
INSERT INTO `eqemu`.`grid_entries` (`gridid`, `zoneid`, `number`, `x`, `y`, `z`, `heading`, `pause`) VALUES ('16', '68', '1', '3202', '855', '-13', '131', '1080');
INSERT INTO `eqemu`.`grid_entries` (`gridid`, `zoneid`, `number`, `x`, `y`, `z`, `heading`, `pause`) VALUES ('16', '68', '2', '3658', '476', '-13', '227', '360');

UPDATE `eqemu`.`grid_entries` SET `pause`='180' WHERE  `zoneid`=96 AND `gridid`=2 AND `number`=4;
UPDATE `eqemu`.`grid_entries` SET `pause`='190' WHERE  `zoneid`=96 AND `gridid`=2 AND `number`=19;

UPDATE `eqemu`.`spawn2` SET `x`='3202', `y`='855', `heading`='131', `pathgrid`='16' WHERE `id`=367975;

INSERT INTO `eqemu`.`grid` (`id`, `zoneid`, `type`, `type2`) VALUES ('16', '68', '3', '1');