#
_XDCBUILDCOUNT = 2
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_5_42_01_09/packages;C:/ti/xdais_7_21_01_07/packages;C:/ti/xdais_7_21_01_07/examples;C:/ti/uia_1_03_01_08/packages;C:/ti/xdctools_3_50_03_33_core/packages;C:/ti/bios_6_42_03_35/packages
override XDCROOT = C:/ti/xdctools_3_50_03_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_5_42_01_09/packages;C:/ti/xdais_7_21_01_07/packages;C:/ti/xdais_7_21_01_07/examples;C:/ti/uia_1_03_01_08/packages;C:/ti/xdctools_3_50_03_33_core/packages;C:/ti/bios_6_42_03_35/packages;C:/ti/xdctools_3_50_03_33_core/packages;..
HOSTOS = Windows
endif
