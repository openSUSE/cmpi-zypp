#!/usr/bin/env python
#
# Example application using powerCIM (http://en.opensuse.org/PowerCIM)
#
# It connects to given host and lists all inatalled and available
# packages.
#
import os
import powerCIM

# Define those 4 variables or write their values to ~/.cmpi-zypp
#
# WBEMServer = 'https://localhost:5989'
# login      = 'root'
# password   = '*******'
# namespace  = 'root/cimv2'
if os.path.exists('%s/.cmpi-zypp'%os.environ['HOME']):
  with open( '%s/.cmpi-zypp'%os.environ['HOME'], 'r' ) as f:
    WBEMServer = f.readline().rstrip()
    login      = f.readline().rstrip()
    password   = f.readline().rstrip()
    namespace  = f.readline().rstrip()

# create factory/connection
factory = powerCIM.WBEMFactory( WBEMServer, login, password, namespace )

# get all installed packages
print '===[installed packages]============================================================'
systems = factory.EnumerateInstances( 'Linux_ComputerSystem' )
for system in systems:
  for package in system._GetAssociators( namespace, 'SUSE_InstalledSoftwareIdentity' ):
    print package.InstanceID

# get all packages
print '===[all packages]============================================================'
instances = factory.EnumerateInstances( 'SUSE_SoftwareIdentity' )
for package in instances:
    print package.InstanceID

