## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/Clock_pe66.oe66

linker.cmd: package/cfg/Clock_pe66.xdl
	$(SED) 's"^\"\(package/cfg/Clock_pe66cfg.cmd\)\"$""\"C:/Users/sitogor/OneDrive - University of Bristol/Documents/Advanced DSP/Advanced-DSP/Clock/.config/xconfig_Clock/\1\""' package/cfg/Clock_pe66.xdl > $@
