; Listing generated by Microsoft (R) Optimizing Compiler Version 19.43.34808.0 

include listing.inc

INCLUDELIB OLDNAMES

PUBLIC	?m_NextUniqueID@sMesh@@0IA			; sMesh::m_NextUniqueID
?m_NextUniqueID@sMesh@@0IA DD 064H			; sMesh::m_NextUniqueID
?_Fake_alloc@std@@3U_Fake_allocator@1@B	ORG $+1		; std::_Fake_alloc
PUBLIC	??0?$allocator@UsLODInfo@sMesh@@@std@@QEAA@XZ	; std::allocator<sMesh::sLODInfo>::allocator<sMesh::sLODInfo>
PUBLIC	??0?$_Vector_val@U?$_Simple_types@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ ; std::_Vector_val<std::_Simple_types<sMesh::sLODInfo> >::_Vector_val<std::_Simple_types<sMesh::sLODInfo> >
PUBLIC	??$?0$$V@?$_Compressed_pair@V?$allocator@UsLODInfo@sMesh@@@std@@V?$_Vector_val@U?$_Simple_types@UsLODInfo@sMesh@@@std@@@2@$00@std@@QEAA@U_Zero_then_variadic_args_t@1@@Z ; std::_Compressed_pair<std::allocator<sMesh::sLODInfo>,std::_Vector_val<std::_Simple_types<sMesh::sLODInfo> >,1>::_Compressed_pair<std::allocator<sMesh::sLODInfo>,std::_Vector_val<std::_Simple_types<sMesh::sLODInfo> >,1><>
PUBLIC	?_Tidy@?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@AEAAXXZ ; std::vector<sMesh::sLODInfo,std::allocator<sMesh::sLODInfo> >::_Tidy
PUBLIC	??0?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ ; std::vector<sMesh::sLODInfo,std::allocator<sMesh::sLODInfo> >::vector<sMesh::sLODInfo,std::allocator<sMesh::sLODInfo> >
PUBLIC	??1?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ ; std::vector<sMesh::sLODInfo,std::allocator<sMesh::sLODInfo> >::~vector<sMesh::sLODInfo,std::allocator<sMesh::sLODInfo> >
PUBLIC	?atan2@@YAMMM@Z					; atan2
PUBLIC	?pFindChildMeshByFriendlyName@sMesh@@QEAAPEAU1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z ; sMesh::pFindChildMeshByFriendlyName
PUBLIC	??0sMesh@@QEAA@XZ				; sMesh::sMesh
;	COMDAT pdata
pdata	SEGMENT
$pdata$?_Tidy@?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@AEAAXXZ DD imagerel $LN31@Tidy
	DD	imagerel $LN31@Tidy+135
	DD	imagerel $unwind$?_Tidy@?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@AEAAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??1?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ DD imagerel $LN35@vector
	DD	imagerel $LN35@vector+135
	DD	imagerel $unwind$??1?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?pFindChildMeshByFriendlyName@sMesh@@QEAAPEAU1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z DD imagerel $LN144@pFindChild
	DD	imagerel $LN144@pFindChild+288
	DD	imagerel $unwind$?pFindChildMeshByFriendlyName@sMesh@@QEAAPEAU1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??0sMesh@@QEAA@XZ DD imagerel $LN174@sMesh
	DD	imagerel $LN174@sMesh+550
	DD	imagerel $unwind$??0sMesh@@QEAA@XZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?dtor$3@?0???0sMesh@@QEAA@XZ@4HA DD imagerel ?dtor$3@?0???0sMesh@@QEAA@XZ@4HA
	DD	imagerel ?dtor$3@?0???0sMesh@@QEAA@XZ@4HA+49
	DD	imagerel $unwind$?dtor$3@?0???0sMesh@@QEAA@XZ@4HA
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$3@?0???0sMesh@@QEAA@XZ@4HA DQ 05002320600020601r ; 2.6336e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$ip2state$??0sMesh@@QEAA@XZ DD 0605e502H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$stateUnwindMap$??0sMesh@@QEAA@XZ DW 0c08H
	DD	imagerel ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QEAA@XZ
	DW	03660H
	DD	imagerel ?dtor$1@?0???0sMesh@@QEAA@XZ@4HA
	DB	02eH
	DD	imagerel ?dtor$2@?0???0sMesh@@QEAA@XZ@4HA
	DB	02eH
	DD	imagerel ?dtor$3@?0???0sMesh@@QEAA@XZ@4HA
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$cppxdata$??0sMesh@@QEAA@XZ DB 028H
	DD	imagerel $stateUnwindMap$??0sMesh@@QEAA@XZ
	DD	imagerel $ip2state$??0sMesh@@QEAA@XZ
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??0sMesh@@QEAA@XZ DQ 0000b340a00040a11r	; 1.55801e-308
	DD	07006720aH
	DD	imagerel __CxxFrameHandler4
	DD	imagerel $cppxdata$??0sMesh@@QEAA@XZ
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?pFindChildMeshByFriendlyName@sMesh@@QEAAPEAU1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z DQ 0000c5416000a1601r ; 1.71448e-308
	DQ	0f0123216000a3416r		; -7.06227e+231
	DQ	0600b700cc00ee010r		; 4.59849e+154
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??1?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@QEAA@XZ DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?_Tidy@?$vector@UsLODInfo@sMesh@@V?$allocator@UsLODInfo@sMesh@@@std@@@std@@AEAAXXZ DQ 03002320600020601r ; 1.96423e-77
END
