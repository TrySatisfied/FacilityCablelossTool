using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FacilityCableTool
{
    class FreqInfo
    {
        public static string[] frequence_2G;
        public static string[] frequence_5G;
        public static string[] frequence_6G;

        public FreqInfo(string[] val1, string[] val2, string[] val3)
        {
            frequence_2G = val1;
            frequence_5G = val2;
            frequence_6G = val3;
        }
    }

    class TabDataItem
    {

        public double FreqMHz { get; set; }
        public double Ant_0 { get; set; }
        public double Ant_1 { get; set; }
        public double Ant_2 { get; set; }
        public double Ant_3 { get; set; }
        public double Ant_4 { get; set; }
        public double Ant_5 { get; set; }
        public double Ant_6 { get; set; }
        public double Ant_7 { get; set; }
    }
    class OtherInfo
    {
        public int portnum { get; set; }

        public OtherInfo(int val)
        {
            portnum = val;
        }
    }
}
