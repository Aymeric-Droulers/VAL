# CMakeList.txt : projet CMake pour VAL-Code, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (VAL-Code "VAL-Code.cpp" "VAL-Code.h" "Rame.hpp" "Station.h" "troncon.hpp" "Rame.cpp" "Station.cpp" "troncon.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VAL-Code PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
