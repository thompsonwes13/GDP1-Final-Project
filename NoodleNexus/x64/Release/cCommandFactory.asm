; Listing generated by Microsoft (R) Optimizing Compiler Version 19.43.34808.0 

include listing.inc

INCLUDELIB OLDNAMES

PUBLIC	??_C@_0CB@JCFOKGAB@Move?5Relative?5ConstVelocity?$CLTim@ ; `string'
PUBLIC	??_C@_0CF@JPCAJHEL@Follow?5Object?0?5offset?5?$CG?5max?5vel@ ; `string'
PUBLIC	??_R2cFollowObject_A@@8				; cFollowObject_A::`RTTI Base Class Array'
PUBLIC	??_R3cMoveRelativeTime@@8			; cMoveRelativeTime::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R1A@?0A@EA@cMoveRelativeTime@@8		; cMoveRelativeTime::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R3iCommand@@8				; iCommand::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2iCommand@@8				; iCommand::`RTTI Base Class Array'
PUBLIC	??_R4cMoveRelativeTime@@6B@			; cMoveRelativeTime::`RTTI Complete Object Locator'
PUBLIC	??_R4iCommand@@6B@				; iCommand::`RTTI Complete Object Locator'
PUBLIC	??_R1A@?0A@EA@cFollowObject_A@@8		; cFollowObject_A::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R4cFollowObject_A@@6B@			; cFollowObject_A::`RTTI Complete Object Locator'
PUBLIC	??_7cMoveRelativeTime@@6B@			; cMoveRelativeTime::`vftable'
PUBLIC	??_R3cFollowObject_A@@8				; cFollowObject_A::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_7cFollowObject_A@@6B@			; cFollowObject_A::`vftable'
PUBLIC	??_R1A@?0A@EA@iCommand@@8			; iCommand::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R0?AVcMoveRelativeTime@@@8			; cMoveRelativeTime `RTTI Type Descriptor'
PUBLIC	??_7iCommand@@6B@				; iCommand::`vftable'
PUBLIC	??_R2cMoveRelativeTime@@8			; cMoveRelativeTime::`RTTI Base Class Array'
PUBLIC	??_R0?AViCommand@@@8				; iCommand `RTTI Type Descriptor'
PUBLIC	??_R0?AVcFollowObject_A@@@8			; cFollowObject_A `RTTI Type Descriptor'
EXTRN	_purecall:PROC
EXTRN	__imp_atof:PROC
EXTRN	__imp__purecall:PROC
;	COMDAT ??_R0?AVcFollowObject_A@@@8
data$rs	SEGMENT
??_R0?AVcFollowObject_A@@@8 DQ FLAT:??_7type_info@@6B@	; cFollowObject_A `RTTI Type Descriptor'
	DQ	0000000000000000H
	DB	'.?AVcFollowObject_A@@', 00H
data$rs	ENDS
;	COMDAT ??_R0?AViCommand@@@8
data$rs	SEGMENT
??_R0?AViCommand@@@8 DQ FLAT:??_7type_info@@6B@		; iCommand `RTTI Type Descriptor'
	DQ	0000000000000000H
	DB	'.?AViCommand@@', 00H
data$rs	ENDS
;	COMDAT ??_R2cMoveRelativeTime@@8
rdata$r	SEGMENT
??_R2cMoveRelativeTime@@8 DD imagerel ??_R1A@?0A@EA@cMoveRelativeTime@@8 ; cMoveRelativeTime::`RTTI Base Class Array'
	DD	imagerel ??_R1A@?0A@EA@iCommand@@8
	ORG $+3
rdata$r	ENDS
;	COMDAT ??_7iCommand@@6B@
CONST	SEGMENT
??_7iCommand@@6B@ DQ FLAT:??_R4iCommand@@6B@		; iCommand::`vftable'
	DQ	FLAT:??_EiCommand@@UEAAPEAXI@Z
	DQ	FLAT:_purecall
	DQ	FLAT:_purecall
	DQ	FLAT:_purecall
	DQ	FLAT:_purecall
CONST	ENDS
;	COMDAT ??_R0?AVcMoveRelativeTime@@@8
data$rs	SEGMENT
??_R0?AVcMoveRelativeTime@@@8 DQ FLAT:??_7type_info@@6B@ ; cMoveRelativeTime `RTTI Type Descriptor'
	DQ	0000000000000000H
	DB	'.?AVcMoveRelativeTime@@', 00H
data$rs	ENDS
;	COMDAT ??_R1A@?0A@EA@iCommand@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@iCommand@@8 DD imagerel ??_R0?AViCommand@@@8 ; iCommand::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	00H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	imagerel ??_R3iCommand@@8
rdata$r	ENDS
;	COMDAT ??_7cFollowObject_A@@6B@
CONST	SEGMENT
??_7cFollowObject_A@@6B@ DQ FLAT:??_R4cFollowObject_A@@6B@ ; cFollowObject_A::`vftable'
	DQ	FLAT:??_EcFollowObject_A@@UEAAPEAXI@Z
	DQ	FLAT:?OnStart@cFollowObject_A@@UEAAXXZ
	DQ	FLAT:?Update@cFollowObject_A@@UEAAXN@Z
	DQ	FLAT:?isFinished@cFollowObject_A@@UEAA_NXZ
	DQ	FLAT:?OnFinished@cFollowObject_A@@UEAAXXZ
CONST	ENDS
;	COMDAT ??_R3cFollowObject_A@@8
rdata$r	SEGMENT
??_R3cFollowObject_A@@8 DD 00H				; cFollowObject_A::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	02H
	DD	imagerel ??_R2cFollowObject_A@@8
rdata$r	ENDS
;	COMDAT ??_7cMoveRelativeTime@@6B@
CONST	SEGMENT
??_7cMoveRelativeTime@@6B@ DQ FLAT:??_R4cMoveRelativeTime@@6B@ ; cMoveRelativeTime::`vftable'
	DQ	FLAT:??_EcMoveRelativeTime@@UEAAPEAXI@Z
	DQ	FLAT:?OnStart@cMoveRelativeTime@@UEAAXXZ
	DQ	FLAT:?Update@cMoveRelativeTime@@UEAAXN@Z
	DQ	FLAT:?isFinished@cMoveRelativeTime@@UEAA_NXZ
	DQ	FLAT:?OnFinished@cMoveRelativeTime@@UEAAXXZ
CONST	ENDS
;	COMDAT ??_R4cFollowObject_A@@6B@
rdata$r	SEGMENT
??_R4cFollowObject_A@@6B@ DD 01H			; cFollowObject_A::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	imagerel ??_R0?AVcFollowObject_A@@@8
	DD	imagerel ??_R3cFollowObject_A@@8
	DD	imagerel ??_R4cFollowObject_A@@6B@
rdata$r	ENDS
;	COMDAT ??_R1A@?0A@EA@cFollowObject_A@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@cFollowObject_A@@8 DD imagerel ??_R0?AVcFollowObject_A@@@8 ; cFollowObject_A::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	01H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	imagerel ??_R3cFollowObject_A@@8
rdata$r	ENDS
;	COMDAT ??_R4iCommand@@6B@
rdata$r	SEGMENT
??_R4iCommand@@6B@ DD 01H				; iCommand::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	imagerel ??_R0?AViCommand@@@8
	DD	imagerel ??_R3iCommand@@8
	DD	imagerel ??_R4iCommand@@6B@
rdata$r	ENDS
;	COMDAT ??_R4cMoveRelativeTime@@6B@
rdata$r	SEGMENT
??_R4cMoveRelativeTime@@6B@ DD 01H			; cMoveRelativeTime::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	imagerel ??_R0?AVcMoveRelativeTime@@@8
	DD	imagerel ??_R3cMoveRelativeTime@@8
	DD	imagerel ??_R4cMoveRelativeTime@@6B@
rdata$r	ENDS
;	COMDAT ??_R2iCommand@@8
rdata$r	SEGMENT
??_R2iCommand@@8 DD imagerel ??_R1A@?0A@EA@iCommand@@8	; iCommand::`RTTI Base Class Array'
	ORG $+3
rdata$r	ENDS
;	COMDAT ??_R3iCommand@@8
rdata$r	SEGMENT
??_R3iCommand@@8 DD 00H					; iCommand::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	01H
	DD	imagerel ??_R2iCommand@@8
rdata$r	ENDS
;	COMDAT ??_R1A@?0A@EA@cMoveRelativeTime@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@cMoveRelativeTime@@8 DD imagerel ??_R0?AVcMoveRelativeTime@@@8 ; cMoveRelativeTime::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	01H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	imagerel ??_R3cMoveRelativeTime@@8
rdata$r	ENDS
;	COMDAT ??_R3cMoveRelativeTime@@8
rdata$r	SEGMENT
??_R3cMoveRelativeTime@@8 DD 00H			; cMoveRelativeTime::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	02H
	DD	imagerel ??_R2cMoveRelativeTime@@8
rdata$r	ENDS
;	COMDAT ??_R2cFollowObject_A@@8
rdata$r	SEGMENT
??_R2cFollowObject_A@@8 DD imagerel ??_R1A@?0A@EA@cFollowObject_A@@8 ; cFollowObject_A::`RTTI Base Class Array'
	DD	imagerel ??_R1A@?0A@EA@iCommand@@8
	ORG $+3
rdata$r	ENDS
;	COMDAT ??_C@_0CF@JPCAJHEL@Follow?5Object?0?5offset?5?$CG?5max?5vel@
CONST	SEGMENT
??_C@_0CF@JPCAJHEL@Follow?5Object?0?5offset?5?$CG?5max?5vel@ DB 'Follow O'
	DB	'bject, offset & max velocity', 00H		; `string'
CONST	ENDS
;	COMDAT ??_C@_0CB@JCFOKGAB@Move?5Relative?5ConstVelocity?$CLTim@
CONST	SEGMENT
??_C@_0CB@JCFOKGAB@Move?5Relative?5ConstVelocity?$CLTim@ DB 'Move Relativ'
	DB	'e ConstVelocity+Time', 00H			; `string'
?_Fake_alloc@std@@3U_Fake_allocator@1@B	ORG $+1		; std::_Fake_alloc
PUBLIC	??$?0NNN@?$vec@$02M$0A@@glm@@QEAA@NNN@Z		; glm::vec<3,float,0>::vec<3,float,0><double,double,double>
PUBLIC	??1cFollowObject_A@@UEAA@XZ			; cFollowObject_A::~cFollowObject_A
PUBLIC	??_GcFollowObject_A@@UEAAPEAXI@Z		; cFollowObject_A::`scalar deleting destructor'
PUBLIC	??1cMoveRelativeTime@@UEAA@XZ			; cMoveRelativeTime::~cMoveRelativeTime
PUBLIC	??_GcMoveRelativeTime@@UEAAPEAXI@Z		; cMoveRelativeTime::`scalar deleting destructor'
PUBLIC	??0cFollowObject_A@@QEAA@XZ			; cFollowObject_A::cFollowObject_A
PUBLIC	?__autoclassinit2@cMoveRelativeTime@@QEAAX_K@Z	; cMoveRelativeTime::__autoclassinit2
PUBLIC	??0iCommand@@QEAA@XZ				; iCommand::iCommand
PUBLIC	??0cMoveRelativeTime@@QEAA@XZ			; cMoveRelativeTime::cMoveRelativeTime
PUBLIC	?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z ; cCommandFactory::pCreateCommandObject
PUBLIC	?setVectorOfMeshes@cCommandFactory@@QEAAXPEAV?$vector@PEAUsMesh@@V?$allocator@PEAUsMesh@@@std@@@std@@@Z ; cCommandFactory::setVectorOfMeshes
PUBLIC	?setPhysics@cCommandFactory@@QEAAXPEAVcPhysics@@@Z ; cCommandFactory::setPhysics
PUBLIC	??0cCommandFactory@@QEAA@XZ			; cCommandFactory::cCommandFactory
PUBLIC	??_GiCommand@@UEAAPEAXI@Z			; iCommand::`scalar deleting destructor'
PUBLIC	??1iCommand@@UEAA@XZ				; iCommand::~iCommand
EXTRN	??_EcFollowObject_A@@UEAAPEAXI@Z:PROC		; cFollowObject_A::`vector deleting destructor'
EXTRN	??_EcMoveRelativeTime@@UEAAPEAXI@Z:PROC		; cMoveRelativeTime::`vector deleting destructor'
EXTRN	??_EiCommand@@UEAAPEAXI@Z:PROC			; iCommand::`vector deleting destructor'
;	COMDAT pdata
pdata	SEGMENT
$pdata$??_GcFollowObject_A@@UEAAPEAXI@Z DD imagerel $LN9@scalar
	DD	imagerel $LN9@scalar+43
	DD	imagerel $unwind$??_GcFollowObject_A@@UEAAPEAXI@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??_GcMoveRelativeTime@@UEAAPEAXI@Z DD imagerel $LN9@scalar
	DD	imagerel $LN9@scalar+43
	DD	imagerel $unwind$??_GcMoveRelativeTime@@UEAAPEAXI@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z DD imagerel $LN267@pCreateCom
	DD	imagerel $LN267@pCreateCom+730
	DD	imagerel $unwind$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??_GiCommand@@UEAAPEAXI@Z DD imagerel $LN9@scalar
	DD	imagerel $LN9@scalar+43
	DD	imagerel $unwind$??_GiCommand@@UEAAPEAXI@Z
;	COMDAT xdata
xdata	SEGMENT
$unwind$??_GiCommand@@UEAAPEAXI@Z DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$ip2state$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z DQ 01d02034d04021d08r ; 5.96617e-169
	DD	03090402H
	DB	00H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$stateUnwindMap$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z DW 0c04H
	DD	imagerel ??1?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@QEAA@XZ
	DW	034a0H
	DD	imagerel ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QEAA@XZ
	DB	090H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$cppxdata$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z DB 028H
	DD	imagerel $stateUnwindMap$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z
	DD	imagerel $ip2state$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z DQ 00006882700113619r ; 9.08365e-309
	DQ	00008681e00077822r		; 1.1691e-308
	DQ	00018341a001b541ar		; 3.36591e-308
	DQ	0e011f0130012011ar		; -6.01266e+154
	DQ	00000600c700dc00fr		; 5.21766e-310
	DD	imagerel __GSHandlerCheck_EH4
	DD	imagerel $cppxdata$?pCreateCommandObject@cCommandFactory@@QEAAPEAViCommand@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@4@@Z
	DD	05aH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??_GcMoveRelativeTime@@UEAAPEAXI@Z DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??_GcFollowObject_A@@UEAAPEAXI@Z DQ 03002320600020601r ; 1.96423e-77
END
