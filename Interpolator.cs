using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.IO;

namespace FacilityCableTool
{
    internal class Interpolator
    {
        public string ReadBandCableLossData(int port, int band)
        {
            string path = string.Format(".\\WT_SETUP\\WT_ATTEN_DUT_{0}.txt", port.ToString());

            if (File.Exists(path))
            {
                string strAttenTableStart = "";
                string strAttenTableEnd = "";

                if (band == 0)
                {
                    strAttenTableStart = "WT_DELTA_ATTEN_2_4_BEGIN";
                    strAttenTableEnd = "WT_DELTA_ATTEN_2_4_END";
                }
                else if (band == 1)
                {
                    strAttenTableStart = "WT_DELTA_ATTEN_5_BEGIN";
                    strAttenTableEnd = "WT_DELTA_ATTEN_5_END";
                }
                else if (band == 2)
                {
                    strAttenTableStart = "WT_DELTA_ATTEN_6_BEGIN";
                    strAttenTableEnd = "WT_DELTA_ATTEN_6_END";
                }

                //File.Open(path, FileMode.Open, FileAccess.Read);
                string data = File.ReadAllText(path);
                data = data.Substring(data.IndexOf(strAttenTableStart));
                data = data.Substring(0, data.IndexOf(strAttenTableEnd));
                return path;
            }
            else
            {
                MessageBoxResult result = MessageBox.Show(String.Format("该路径{0}下的文件不存在", path), "警告", MessageBoxButton.OK);
                return "";
            }
        }

        public bool WriteAtten(int channel)
        {
            return true;
        }
    }
}
