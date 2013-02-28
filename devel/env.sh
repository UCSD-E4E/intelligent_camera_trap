#!/usr/bin/env sh
# generated from catkin/cmake/templates/env.sh.in

if [ $# -eq 0 ] ; then
  /bin/echo "Entering environment at '/home/viki/groovy_workspace/ICT_Viper/devel', type 'exit' to leave"
  . "/home/viki/groovy_workspace/ICT_Viper/devel/setup.sh"
  "$SHELL" -i
  /bin/echo "Exiting environment at '/home/viki/groovy_workspace/ICT_Viper/devel'"
else
  . "/home/viki/groovy_workspace/ICT_Viper/devel/setup.sh"
  exec "$@"
fi
