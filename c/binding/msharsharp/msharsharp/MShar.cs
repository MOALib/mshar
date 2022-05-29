using System;
using System.Runtime.InteropServices;

namespace MXPSQL.MSharSharp{
    public static class MShar{
        private static readonly bool is64bit = IntPtr.Size == 8;

        [DllImport("libmshar", EntryPoint="mkmshar")]
        private static extern String _mkmshar(String prescript, String postscript, String[] files, UIntPtr size);

        public static String mkmshar(String prescript, String postscript, String[] files){
            return _mkmshar(prescript, postscript, files, (UIntPtr)files.Length);
        }
    }
}