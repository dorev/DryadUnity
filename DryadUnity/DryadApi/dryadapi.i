%module DryadApi
%{
#include "dryadapi.h"
%}
%include <windows.i>
%include "std_string.i"
%include "std_vector.i"
%include "dryadapi.h"

%template(MotifList) std::vector<MotifNote>;
%template(NodeList) std::vector<Node>;
%template(EdgeList) std::vector<Edge>;
%template(IntervalList) std::vector<unsigned>;
%template(NoteToPlayList) std::vector<NoteToPlay>;