
INSTALL( FILES
	SUSE_SoftwareIdentity.mof
	SUSE_SoftwareIdentity.sfcb.reg
	SUSE_InstalledSoftwareIdentity.mof
	SUSE_InstalledSoftwareIdentity.sfcb.reg
	SUSE_SystemSpecificCollection.mof
	SUSE_SystemSpecificCollection.sfcb.reg
	SUSE_MemberOfCollection.sfcb.reg
	SUSE_MemberOfCollection.mof
	SUSE_HostedCollection.sfcb.reg
	SUSE_HostedCollection.mof
	SUSE_SoftwareInstallationService.mof
	SUSE_SoftwareInstallationService.sfcb.reg
	deploy.mof
	DESTINATION ${CMPIZYPP_DATA_DIR}/
	)
