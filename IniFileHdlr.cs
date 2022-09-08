using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.IO;

namespace FacilityCableTool
{
    internal class IniFileHdlr
    {
    }
    public partial class InifileHandlr
    {
        [DllImport("kernel32")]//返回0表示失败，非0为成功
        private static extern long WritePrivateProfileString(string section, string key,
            string val, string filePath);

        [DllImport("kernel32")]//返回取得字符串缓冲区的长度
        private static extern long GetPrivateProfileString(string section, string key,
            string def, StringBuilder retVal, int size, string filePath);

        public string ReadIniData(string Section, string Key)//读取INI文件
        {
            string str = System.Environment.CurrentDirectory;//获取当前文件目录
            //ini文件路径
            string str1 = "" + str + "\\config.ini";
            if (File.Exists("" + str1 + ""))
            {
                StringBuilder temp = new StringBuilder(1024);
                GetPrivateProfileString(Section, Key, null, temp, 1024, str1);
                return temp.ToString();
            }
            else
            {
                return String.Empty;
            }
        }

        public static void WriteIniData(string section, string key, string val)
        {
            string str = System.Environment.CurrentDirectory;//获取当前文件目录
            //ini文件路径
            string str1 = "" + str + "\\config.ini";
            if (File.Exists("" + str1 + ""))
            {
                WritePrivateProfileString(section, key, val, str1);
            }
        }

    }
}
