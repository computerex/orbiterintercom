#ifndef _H_ORBITER_INTERCOM_COMMON_STUFF_H_
#define _H_ORBITER_INTERCOM_COMMON_STUFF_H_

// This file has macros/typedefs shared by the OICOM core as well as the exported
// interface oicominterface.lib

// OICOM uses semantic versioning. A summar follows. For details check out http://semver.org/
// version format is as follows:
// major.minor.patch
// increments to the major version indicates a backwards incompatible change
// minor version increments indicate new functionality or other significant backwards compatible changes
// and patch version increments indicate backwards compatible bug fixes
// increments to major version resets minor and patch versions to 0, increments to minor 
// version resets patch version to 0

#define OICOM_VERSION           "0.2.0"

// plug-in id
typedef int OICOM_PID;     

#define PLUGIN_REMOVED         -3
#define PLUGIN_ALREADY_EXISTS  -1
#define PLUGIN_INVALID         -2
#define PLUGIN_NAME_TOO_SHORT  -4

#define MESSAGE_DELETE_OK       1
#define MESSAGE_OK              0
#define MESSAGE_NONE_EXISTS    -1

#endif
