#ifndef DND_DUNGEONUTIL_IN
#define DND_DUNGEONUTIL_IN

// Called from a map's own library, whose string ids differ from ours: compare the text, not the id.
Script "DnD Fetch Constant" (raw constant_label) {
    int result = 0;
    if(!StrCmp(constant_label, "PTID"))
        result = P_TIDSTART;
    SetResultValue(result);
}

#endif