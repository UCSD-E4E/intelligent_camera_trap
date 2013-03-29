#!/usr/bin/env sh
# generated from catkin/cmake/templates/env.sh.in

if [ $# -eq 0 ] ; then
  /bin/echo "Entering environment at '/home/riley/fuerte_workspace/sandbox/ICT_Viper/devel', type 'exit' to leave"
  . "/home/riley/fuerte_workspace/sandbox/ICT_Viper/devel/setup.sh"
  "$SHELL" -i
  /bin/echo "Exiting environment at '/home/riley/fuerte_workspace/sandbox/ICT_Viper/devel'"
else
  . "/home/riley/fuerte_workspace/sandbox/ICT_Viper/devel/setup.sh"
  exec "$@"
fi
