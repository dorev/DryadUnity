%module DryadApi
%{
#include "dryadapi.h"
%}
%include <windows.i>
%include "std_string.i"
%include "std_vector.i"
%include "dryadapi.h"

%template(NodeVector) std::vector<Node>;
%template(EdgeVector) std::vector<Edge>;
%template(IntVector) std::vector<int>;
%template(NoteConcreteVector) std::vector<NoteConcrete>;
%template(NoteConceptVector) std::vector<NoteConcept>;