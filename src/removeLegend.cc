#include <TH1.h>
#include <TList.h>

void removeLegend(TH1* h) {
    TList* primitives = h->GetListOfPrimitives();
    TIter next(primitives);
    while (TObject* obj = next()) {
        if (obj->InheritsFrom("TLegend")) {
            primitives->Remove(obj);
            break;  // there should only be one legend
        }
    }
    h->Update();
}
