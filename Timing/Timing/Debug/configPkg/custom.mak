## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/Sys_BIOs_config_pe66.oe66

linker.cmd: package/cfg/Sys_BIOs_config_pe66.xdl
	$(SED) 's"^\"\(package/cfg/Sys_BIOs_config_pe66cfg.cmd\)\"$""\"C:/Users/eend/OneDrive - University of Bristol/2023_24/EENGM4120/Timer/Timing/Debug/configPkg/\1\""' package/cfg/Sys_BIOs_config_pe66.xdl > $@