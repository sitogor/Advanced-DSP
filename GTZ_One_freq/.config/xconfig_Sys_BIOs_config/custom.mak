## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/Sys_BIOs_config_pe66.oe66

linker.cmd: package/cfg/Sys_BIOs_config_pe66.xdl
	$(SED) 's"^\"\(package/cfg/Sys_BIOs_config_pe66cfg.cmd\)\"$""\"C:/Users/Tszyau/Desktop/Advanced_DSP_FPGA/Coursework 1/Advanced-DSP/GTZ_One_freq/.config/xconfig_Sys_BIOs_config/\1\""' package/cfg/Sys_BIOs_config_pe66.xdl > $@
