#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/led.c src/main.c src/lab03.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/led.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/lab03.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/led.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/lab03.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/led.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/lab03.o

# Source Files
SOURCEFILES=src/led.c src/main.c src/lab03.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256MC710
MP_LINKER_FILE_OPTION=,--script=p33FJ256MC710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/led.o: src/led.c  .generated_files/flags/default/2814c93279d31c7d43da7e45dbf34ccd720570f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/led.o.d 
	@${RM} ${OBJECTDIR}/src/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/led.c  -o ${OBJECTDIR}/src/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/led.o.d"      -g -D__DEBUG     -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/default/2902b4839002e693bb98740f90b6c37165db304c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG     -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/lab03.o: src/lab03.c  .generated_files/flags/default/ce86f3079311c09f21c6a5e3740c0152d436feb2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lab03.o.d 
	@${RM} ${OBJECTDIR}/src/lab03.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/lab03.c  -o ${OBJECTDIR}/src/lab03.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/lab03.o.d"      -g -D__DEBUG     -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/src/led.o: src/led.c  .generated_files/flags/default/1b39cf00d62a8b85a5804cdff327ec2648a4af52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/led.o.d 
	@${RM} ${OBJECTDIR}/src/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/led.c  -o ${OBJECTDIR}/src/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/led.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/flags/default/23c0921f65fb62619220501180331a44a614ddea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/src/lab03.o: src/lab03.c  .generated_files/flags/default/e054a3081c691033a0dc9f79a93eb1e4b4346b1e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lab03.o.d 
	@${RM} ${OBJECTDIR}/src/lab03.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/lab03.c  -o ${OBJECTDIR}/src/lab03.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/lab03.o.d"        -g -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  lib/libLCD.a  
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    lib/libLCD.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include"     -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  lib/libLCD.a 
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    lib/libLCD.a  -mcpu=$(MP_PROCESSOR_OPTION)        -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"include" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/Lab03_DAC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a    
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}
