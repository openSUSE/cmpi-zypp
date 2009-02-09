
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
	SUSE_InstallationServiceAffectsSoftwareIdentity.mof
	SUSE_InstallationServiceAffectsSoftwareIdentity.sfcb.reg
	SUSE_SoftwareInstallationServiceCapabilities.mof
	SUSE_SoftwareInstallationServiceCapabilities.sfcb.reg
        SUSE_ElementCapabilities.mof
        SUSE_ElementCapabilities.sfcb.reg
        SUSE_HostedInstallationService.mof
        SUSE_HostedInstallationService.sfcb.reg
	SUSE_SoftwareRepository.mof
	SUSE_SoftwareRepository.sfcb.reg
        SUSE_HostedRepository.mof
        SUSE_HostedRepository.sfcb.reg
        SUSE_InstallationServiceRepositoryDependency.mof
        SUSE_InstallationServiceRepositoryDependency.sfcb.reg
	deploy.mof
	DESTINATION ${CMPIZYPP_DATA_DIR}/
	)
