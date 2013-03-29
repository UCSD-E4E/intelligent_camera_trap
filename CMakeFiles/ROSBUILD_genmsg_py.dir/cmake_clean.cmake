FILE(REMOVE_RECURSE
  "msg_gen"
  "srv_gen"
  "src/smart-camera-trap/msg"
  "src/smart-camera-trap/srv"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "src/smart-camera-trap/msg/__init__.py"
  "src/smart-camera-trap/msg/_Coords.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
