#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_35_04_50/packages;C:/ti/ccsv5/ccs_base;C:/Users/sitogor/ONEDRI~1/DOCUME~1/ADVANC~1/Advanced-DSP/Clock/.config
override XDCROOT = C:/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_35_04_50/packages;C:/ti/ccsv5/ccs_base;C:/Users/sitogor/ONEDRI~1/DOCUME~1/ADVANC~1/Advanced-DSP/Clock/.config;C:/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Windows
endif
