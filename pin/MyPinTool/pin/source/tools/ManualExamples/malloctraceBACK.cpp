// #include "pin.H"
// void * MallocWrapper(AFUNPTR pf_malloc, size_t size) { // Simulate out-of-memory every so often
//     //void * res;
//     if (1)
//     return (NULL);
//     // res = pf_malloc(size);
//     // return res;
// }

// VOID ImageLoad (IMG img, VOID *v) {
//     if (strstr(IMG_Name(img).c_str(), "libc.so") ||
//     strstr(IMG_Name(img).c_str(), "MSVCR80") || strstr(IMG_Name(img).c_str(), "MSVCR90")) {

//         RTN mallocRtn = RTN_FindByName(img, "malloc");
//         if ( RTN_Valid(mallocRtn) && RTN_IsSafeForProbedReplacement(mallocRtn) ) {
//             PROTO proto_malloc = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT, "malloc", 
//             PIN_PARG(size_t), PIN_PARG_END() );
//             RTN_ReplaceSignatureProbed (mallocRtn, 
//             AFUNPTR(MallocWrapper),
//             IARG_PROTOTYPE, proto_malloc,
//             IARG_ORIG_FUNCPTR,
//             IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
//             IARG_END);
//         } 
//     }
// }

// int main(int argc, CHAR *argv[]) {
//     PIN_InitSymbols(); PIN_Init(argc,argv));
//     IMG_AddInstrumentFunction(ImageLoad, 0);
//     PIN_StartProgramProbed();
// }