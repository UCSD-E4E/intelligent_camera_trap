#!/usr/bin/env sh
# THIS IS AN AUTO-GENERATED FILE
# IT IS UNLIKELY YOU WANT TO EDIT THIS FILE BY HAND
# IF YOU WANT TO CHANGE THE ROS ENVIRONMENT VARIABLES
# USE THE rosinstall OR rosws TOOL INSTEAD.
# see: http://www.ros.org/wiki/rosinstall


export ROS_WORKSPACE=/home/riley/fuerte_workspace/sandbox/ICT_Viper
if [ ! "$ROS_MASTER_URI" ] ; then export ROS_MASTER_URI=http://localhost:11311 ; fi
unset ROS_ROOT

unset _SETUP_SH_ERROR

# python script to read .rosinstall even when rosinstall is not installed
# this files parses the .rosinstall and sets environment variables accordingly
# The ROS_PACKAGE_PATH contains all elements in reversed order (for historic reasons)

# We store into _PARSED_CONFIG the result of python code,
# which is the ros_package_path and the list of setup_files to source
# Using python here to benefit of the pyyaml library
export _PARSED_CONFIG=`/usr/bin/env python << EOPYTHON

import sys
import os
import yaml

workspace_path = os.environ.get('ROS_WORKSPACE', os.path.abspath('.'))
filename = os.path.join(workspace_path, '.rosinstall')

if not os.path.isfile(filename):
    sys.exit("There is no file at %s" % filename)

with open(filename, "r") as fhand:
  try:
    v = fhand.read();
  except Exception as e:
    sys.exit("Failed to read file: %s %s " % (filename, str(e)))

try:
  y = yaml.load(v);
except Exception as e:
  sys.exit("Invalid yaml in %s: %s " % (filename, str(e)))

if y is not None:

  # put all non-setupfile entries into ROS_PACKAGE_PATH
  paths = []
  for vdict in y:
    for k, v in vdict.items():
      if v is not None and k != "setup-file":
        path = os.path.join(workspace_path, v['local-name'])
        if not os.path.isfile(path):
          # add absolute path from workspace to relative paths
          paths.append(os.path.normpath(path))
        else:
          sys.stderr.write("ERROR: referenced path is a file, not a folder: %s" % path)
  output = ''
  # add paths in reverse order
  if len(paths) > 0:
    output += ':'.join(reversed(paths))

  # We also want to return the location of any setupfile elements
  output += 'ROSINSTALL_PATH_SETUPFILE_SEPARATOR'
  setupfile_paths = []
  for vdict in y:
    for k, v in vdict.items():
      if v is not None and k == "setup-file":
        path = os.path.join(workspace_path, v['local-name'])
        if not os.path.exists(path):
          sys.stderr.write("WARNING: referenced setupfile does not exist: %s" % path)
        elif os.path.isfile(path):
          setupfile_paths.append(path)
        else:
          sys.stderr.write("ERROR: referenced setupfile is a folder: %s" % path)
  output += ':'.join(setupfile_paths)

  # printing will store the result in the variable
  print(output)
EOPYTHON`

if [ -z "${_PARSED_CONFIG}" ]; then
  echo 'Could not parse .rosinstall file'
  _SETUP_SH_ERROR=1
fi

# whitespace separates ros_package_path and setupfile results, using sed to split them up
export _ROS_PACKAGE_PATH_ROSINSTALL=`echo "$_PARSED_CONFIG" | sed 's,\(.*\)ROSINSTALL_PATH_SETUPFILE_SEPARATOR\(.*\),\1,'`
_SETUPFILES_ROSINSTALL=`echo "$_PARSED_CONFIG" | sed 's,\(.*\)'ROSINSTALL_PATH_SETUPFILE_SEPARATOR'\(.*\),\2,'`
unset _PARSED_CONFIG

# reset RPP before running setup files
export ROS_PACKAGE_PATH=

# colon separates entries
_LOOP_SETUP_FILE=`echo $_SETUPFILES_ROSINSTALL | sed 's,\([^:]*\)[:]\(.*\),\1,'`
while [ ! -z "$_LOOP_SETUP_FILE" ]
do
  if [ -f "$_LOOP_SETUP_FILE" ]; then
    . $_LOOP_SETUP_FILE
  else
    echo warn: no such file : "$_LOOP_SETUP_FILE"
  fi
  _SETUPFILES_ROSINSTALL=`echo $_SETUPFILES_ROSINSTALL | sed 's,\([^:]*[:]*\),,'`
  _LOOP_SETUP_FILE=`echo $_SETUPFILES_ROSINSTALL | sed 's,\([^:]*\)[:]\(.*\),\1,'`
done

unset _LOOP_SETUP_FILE
unset _SETUPFILES_ROSINSTALL
# restore ROS_WORKSPACE in case other setup.sh changed/unset it
export ROS_WORKSPACE=/home/riley/fuerte_workspace/sandbox/ICT_Viper

# prepend elements from .rosinstall file to ROS_PACKAGE_PATH
# removing existing duplicates entries from value set by setup files
export ROS_PACKAGE_PATH=`/usr/bin/env python << EOPYTHON
import os

ros_package_path1 = os.environ.get('ROS_PACKAGE_PATH', '')
original_elements = ros_package_path1.split(':')
ros_package_path2 = os.environ.get('_ROS_PACKAGE_PATH_ROSINSTALL', '')
new_elements = ros_package_path2.split(':')
for original_path in original_elements:
  if original_path and original_path not in new_elements:
    new_elements.append(original_path)
print(':'.join(new_elements))
EOPYTHON`

unset _ROS_PACKAGE_PATH_ROSINSTALL

# if setup.sh did not set ROS_ROOT (pre-fuerte)
if [ -z "${ROS_ROOT}" ]; then
  # using ROS_ROOT now being in ROS_PACKAGE_PATH
  export _ROS_ROOT_ROSINSTALL=`/usr/bin/env python << EOPYTHON
import sys, os;
if 'ROS_PACKAGE_PATH' in os.environ:
  pkg_path = os.environ['ROS_PACKAGE_PATH']
  for path in pkg_path.split(':'):
    if (os.path.basename(path) == 'ros'
        and os.path.isfile(os.path.join(path, 'stack.xml'))):
      print(path)
      break
EOPYTHON`

  if [ ! -z "${_ROS_ROOT_ROSINSTALL}" ]; then
    export ROS_ROOT=$_ROS_ROOT_ROSINSTALL
    export PATH=$ROS_ROOT/bin:$PATH
    export PYTHONPATH=$ROS_ROOT/core/roslib/src:$PYTHONPATH
  fi
unset _ROS_ROOT_ROSINSTALL
fi

if [ ! -z "$_SETUP_SH_ERROR" ]; then
  # return failure code when sourcing file
  false
fi
