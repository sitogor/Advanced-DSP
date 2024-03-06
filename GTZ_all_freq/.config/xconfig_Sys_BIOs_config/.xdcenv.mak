#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/ti/bios_6_35_04_50/packages;D:/ti/ccsv5/ccs_base;D:/ti/uia_1_03_01_08/packages;D:/Data_And_Docs/University/2nd Year/C Coursework/RepoCopy/C_embedded/GTZ_all_freq/.config
override XDCROOT = D:/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/ti/bios_6_35_04_50/packages;D:/ti/ccsv5/ccs_base;D:/ti/uia_1_03_01_08/packages;D:/Data_And_Docs/University/2nd Year/C Coursework/RepoCopy/C_embedded/GTZ_all_freq/.config;D:/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Windows
endif
