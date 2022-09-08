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
using DialogBox;

namespace FacilityCableTool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        public bool isimported = false;
        FreqInfo FreqInfo;
        OtherInfo OtherInfo;
        InifileHandlr InifileHandlr;
        Comparison<TabDataItem> comparison;
        Interpolator Interpolator = new Interpolator();
        List<List<TabDataItem>> data = new List<List<TabDataItem>>();
        [DllImport("common.dll")] public static extern double ChangeChannelToFreqHz(int enable, int v, int c);
        [DllImport("common.dll")] public static extern int ChangeFreqHzToChannel(int demode, int mcs, double FreqHz);
        [DllImport("common.dll")] public static extern bool CreateObject(int val);
        [DllImport("common.dll")] unsafe public static extern bool bGetExtAtten(int nFreqGrp, int nAnt, int nChannel, double* pRetValue);
        [DllImport("common.dll")] unsafe public static extern bool bSetExtAtten(int nFreqGrp, int nAnt, double dAtten);
        [DllImport("common.dll")] unsafe public static extern bool bSetExtAtten_DiffChannel(int nFreqGrp, int nAnt, int nChannel, double dExtAtten_DiffChannel_dB);


        public MainWindow()
        {
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            InitializeComponent();
            Init();
        }
        public unsafe void Init()
        {
            //Init parameter
            InifileHandlr = new InifileHandlr();
            porttext.Foreground = Brushes.Blue;
            comparison = new Comparison<TabDataItem>((TabDataItem x, TabDataItem y) =>
            {
                if (x.FreqMHz < y.FreqMHz)
                    return -1;
                else if (x.FreqMHz == y.FreqMHz)
                    return 0;
                else
                    return 1;
            });

            //Init extern file
            string port = InifileHandlr.ReadIniData("SETTINGS", "portnum");
            bool flag = CreateObject(Convert.ToInt32(port));

            if (!flag)
            {
                printfInfo("初始化线衰文本失败，请检查端口号是否正确（1~8）或线衰文件是否存在（.\\WT_SETUP\\)");
                MessageBoxResult result = MessageBox.Show("初始化线衰文本失败，请检查端口号是否正确（1~8）或线衰文件是否存在（.\\WT_SETUP\\)", "警告", MessageBoxButton.OK);
                this.Close();
            }
            else
            {
                porttext.Text = porttext.Text + port;
                printfInfo("Program initialization success!");
                InifileHandlr.WriteIniData("MEMORY", "importflag_2G", "NoImport");
                InifileHandlr.WriteIniData("MEMORY", "importflag_5G", "NoImport");
                InifileHandlr.WriteIniData("MEMORY", "importflag_6G", "NoImport");
            }

            //Process data
            List<string[]> freq = new List<string[]>();
            string[] freqinfo = new string[] { "freqHz_2G", "freqhz_5G", "freqhz_6G" };
            for (int i = 0; i < 3; i++)
            {
                freq.Add(InifileHandlr.ReadIniData("MEMORY", freqinfo[i]).Split('|'));
            }

            FreqInfo = new FreqInfo(freq[0], freq[1], freq[2]);
            OtherInfo = new OtherInfo(Convert.ToInt32((InifileHandlr.ReadIniData("SETTINGS", "portnum"))));
            OtherInfo.portnum = Convert.ToInt32(port);

            data.Add(new List<TabDataItem>());
            for (int i = 0; i < FreqInfo.frequence_2G.Count(); i++)
            {
                data[0].Add(new TabDataItem() { FreqMHz = Convert.ToDouble(FreqInfo.frequence_2G[i]) });
            }
            dataGrid_2G.ItemsSource = data[0];
            data[0].Sort(comparison);

            data.Add(new List<TabDataItem>());
            for (int i = 0; i < FreqInfo.frequence_5G.Count(); i++)
            {
                data[1].Add(new TabDataItem() { FreqMHz = Convert.ToDouble(FreqInfo.frequence_5G[i]) });
            }
            dataGrid_5G.ItemsSource = data[1];
            data[1].Sort(comparison);

            data.Add(new List<TabDataItem>());
            for (int i = 0; i < FreqInfo.frequence_6G.Count(); i++)
            {
                data[2].Add(new TabDataItem() { FreqMHz = Convert.ToDouble(FreqInfo.frequence_6G[i]) });
            }
            dataGrid_6G.ItemsSource = data[2];
            data[2].Sort(comparison);

        }
        public void printfInfo(string str)
        {
            infolist.Items.Add(str);
            //Decorator decorator = (Decorator)VisualTreeHelper.GetChild(infolist, 0);
            //ScrollViewer scrollViewer = (ScrollViewer)decorator.Child;
            //scrollViewer.ScrollToEnd();
            scroll.ScrollToEnd();
            //infolist.ScrollIntoView(str);
            //infolist.ScrollIntoView(infolist.Items[infolist.Items.Count - 1]);
        }

        public bool LossErrJudge()
        {
            //Get band loss error
            int idx = tabControl.SelectedIndex;
            double err = Convert.ToDouble(InifileHandlr.ReadIniData("SETTINGS", string.Format("losserr_{0}G", idx == 0 ? "2" : (idx == 1) ? "5" : "6")));
            int size = data[tabControl.SelectedIndex].Count;
            double[,] array = new double[size, 8];

            for (int i = 0; i < size; i++)
            {
                array[i, 0] = data[tabControl.SelectedIndex][i].Ant_0;
                array[i, 1] = data[tabControl.SelectedIndex][i].Ant_1;
                array[i, 2] = data[tabControl.SelectedIndex][i].Ant_2;
                array[i, 3] = data[tabControl.SelectedIndex][i].Ant_3;
                array[i, 4] = data[tabControl.SelectedIndex][i].Ant_4;
                array[i, 5] = data[tabControl.SelectedIndex][i].Ant_5;
                array[i, 6] = data[tabControl.SelectedIndex][i].Ant_6;
                array[i, 7] = data[tabControl.SelectedIndex][i].Ant_7;
            }

            string infostr = "";
            for (int ant = 0; ant < 8; ant++)
            {
                int pmax = 0, pmin = 0;
                double lmax = -9999, lmin = 9999;

                for (int poi = 0; poi < size; poi++)
                {
                    double loss = array[poi, ant];
                    if (lmin >= loss)
                    {
                        lmin = loss;
                        pmin = poi;
                    }

                    if (lmax <= loss)
                    {
                        lmax = loss;
                        pmax = poi;
                    }
                    if (lmax - lmin >= err)
                    {
                        infostr += String.Format("{0} Ant{1} {2}:{3}dbm,{4}:{5}dbm 频点间误差超过{6}dbm\n", idx == 0 ? "2.4G" : (idx == 1) ? "5G" : "6G", ant, data[tabControl.SelectedIndex][pmin].FreqMHz, lmin, data[tabControl.SelectedIndex][pmax].FreqMHz, lmax, err);
                        break;
                    }
                }
            }

            if (infostr.Length > 0)
            {
                MessageBoxResult result = MessageBox.Show(String.Format(infostr + "\n以上频点之间的误差不符合预设值，仍然导入线损数据吗?"), "警告", MessageBoxButton.YesNo, MessageBoxImage.Question);
                if (result == MessageBoxResult.Yes)
                {
                    return true;
                }
                else
                    return false;
            }
            return true;
        }
        private void tabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void Closed_AskIfSave(object sender, System.ComponentModel.CancelEventArgs e)
        {
            MessageBoxResult result = MessageBox.Show("确定保存频点数据吗？", "询问", MessageBoxButton.YesNo, MessageBoxImage.Question);

            string[] val = new string[3];
            //关闭窗口
            if (result == MessageBoxResult.Yes)
            {
                for (int i = 0; i < data[0].Count(); i++)
                    val[0] += data[0][i].FreqMHz + (i != data[0].Count() - 1 ? "|" : "");

                for (int i = 0; i < data[1].Count(); i++)
                    val[1] += data[1][i].FreqMHz + (i != data[1].Count() - 1 ? "|" : "");

                for (int i = 0; i < data[2].Count(); i++)
                    val[2] += data[2][i].FreqMHz + (i != data[2].Count() - 1 ? "|" : "");

                string[] freqinfo = new string[] { "freqHz_2G", "freqhz_5G", "freqhz_6G" };
                for (int i = 0; i < freqinfo.Length; i++)
                    InifileHandlr.WriteIniData("MEMORY", freqinfo[i], val[i]);
            }
            e.Cancel = false;
        }

        private void Add_Clicked(object sender, RoutedEventArgs e)
        {
            var idx = tabControl.SelectedIndex;
            double min = 2412;
            double max = 2801;
            switch (idx)
            {
                case 0:
                    min = 2412;
                    max = 2484;
                    break;
                case 1:
                    min = 4980;
                    max = 5905;
                    break;
                case 2:
                    min = 5935;
                    max = 7115;
                    break;
            }
            var dlg = new InputWindow
            {
                Owner = this,
                maxfre = max,
                minfre = min,
            };
            dlg.ShowDialog();
            if (dlg.addresult)
            {
                foreach (var obj in data[idx])
                {
                    if (obj.FreqMHz == dlg.freqval)
                        return;
                }
                data[idx].Add(new TabDataItem() { FreqMHz = dlg.freqval });
                data[idx].Sort(comparison);

                switch (idx)
                {
                    case 0:
                        dataGrid_2G.ItemsSource = null;
                        dataGrid_2G.ItemsSource = data[idx];
                        break;
                    case 1:
                        dataGrid_5G.ItemsSource = null;
                        dataGrid_5G.ItemsSource = data[idx];
                        break;
                    case 2:
                        dataGrid_6G.ItemsSource = null;
                        dataGrid_6G.ItemsSource = data[idx];
                        break;
                }
            }
        }

        private void Import_Clicked(object sender, RoutedEventArgs e)
        {
            //Judge
            if (data[tabControl.SelectedIndex].Count == 0)
            {
                printfInfo("频点数为0，请重新配置当前频段的频点！");
                return;
            }
            else
            {
                if (!LossErrJudge())
                    return;
            }

            //Set fixed cable loss and diff cable loss
            int idx = tabControl.SelectedIndex != 2 ? tabControl.SelectedIndex : 7;
            double[,] bandFreqRange = new double[3, 2] { { 2412, 2484 }, { 4920, 5905 }, { 5955, 7115 } };
            for (int ant = 0; ant < 8; ant++)
            {
                bSetExtAtten(idx, ant, 0);
                printfInfo(String.Format("Set {0} Ant{1} fixed cable loss {2:0.0} dbm.",
                    (idx != 7 ? (idx == 0 ? "2G" : "5G") : "6G"), ant, 0));

                double firstFreq = data[tabControl.SelectedIndex][0].FreqMHz;
                double lastFreq = data[tabControl.SelectedIndex].Last().FreqMHz;

                //Interpolate first frequence point in front of cable loss
                for (double freq = bandFreqRange[tabControl.SelectedIndex, 0]; freq <= bandFreqRange[tabControl.SelectedIndex, 1]; freq += 5)
                {
                    int channel = ChangeFreqHzToChannel(0, 0, freq * 1e6);

                    if (freq <= firstFreq)
                    {
                        bool flag = bSetExtAtten_DiffChannel(idx, ant, channel, GetLoss(0, ant));
                        if (flag)
                        {
                            printfInfo(String.Format("Set {0} Ant{1} {2} MHz(Ch:{3}) diff cable loss {4:0.0} dbm.", (idx != 7 ? (idx == 0 ? "2G" : "5G") : "6G"), ant, freq, channel, GetLoss(0, ant)));
                        }
                    }
                    else
                        break;
                }

                //Interpolate the rest of frequence point cable loss
                for (int i = 0; i < data[tabControl.SelectedIndex].Count; i++)
                {
                    if (i == data[tabControl.SelectedIndex].Count - 1)
                    {
                        for (double freq = data[tabControl.SelectedIndex][i].FreqMHz; freq <= bandFreqRange[tabControl.SelectedIndex, 1]; freq += 5)
                        {
                            int channel = ChangeFreqHzToChannel(0, 0, freq * 1e6);
                            bool flag = bSetExtAtten_DiffChannel(idx, ant, channel, GetLoss(i, ant));
                            if (flag)
                                printfInfo(String.Format("Set {0} Ant{1} {2} MHz(Ch:{3}) diff cable loss {4:0.0} dbm.", (idx != 7 ? (idx == 0 ? "2G" : "5G") : "6G"), ant, freq, channel, GetLoss(i, ant)));
                        }
                    }
                    else
                    {
                        for (double freq = data[tabControl.SelectedIndex][i].FreqMHz; freq <= data[tabControl.SelectedIndex][i + 1].FreqMHz; freq += 5)
                        {
                            int channel = ChangeFreqHzToChannel(0, 0, freq * 1e6);
                            bool flag = bSetExtAtten_DiffChannel(idx, ant, channel, GetLoss(i, ant));
                            if (flag)
                                printfInfo(String.Format("Set {0} Ant{1} {2} MHz(Ch:{3}) diff cable loss {4:0.0} dbm.", (idx != 7 ? (idx == 0 ? "2G" : "5G") : "6G"), ant, freq, channel, GetLoss(i, ant)));
                        }
                    }
                }
            }

            switch (tabControl.SelectedIndex)
            {
                case 0:
                    textimport2G.Foreground = Brushes.Green;
                    textimport2G.Text = "2G:Imported";
                    InifileHandlr.WriteIniData("MEMORY", "importflag_2G", "Imported");
                    break;
                case 1:
                    textimport5G.Foreground = Brushes.Green;
                    textimport5G.Text = "5G:Imported";
                    InifileHandlr.WriteIniData("MEMORY", "importflag_5G", "Imported");
                    break;
                case 2:
                    textimport6G.Foreground = Brushes.Green;
                    textimport6G.Text = "6G:Imported";
                    InifileHandlr.WriteIniData("MEMORY", "importflag_6G", "Imported");
                    break;
            }
        }

        private void Del_Clicked(object sender, RoutedEventArgs e)
        {
            var idx = tabControl.SelectedIndex;

            switch (idx)
            {
                case 0:
                    data[idx].Remove((TabDataItem)dataGrid_2G.SelectedItem);
                    dataGrid_2G.ItemsSource = null;
                    dataGrid_2G.ItemsSource = data[idx];
                    break;
                case 1:
                    data[idx].Remove((TabDataItem)dataGrid_5G.SelectedItem);
                    dataGrid_5G.ItemsSource = null;
                    dataGrid_5G.ItemsSource = data[idx];
                    break;
                case 2:
                    data[idx].Remove((TabDataItem)dataGrid_6G.SelectedItem);
                    dataGrid_6G.ItemsSource = null;
                    dataGrid_6G.ItemsSource = data[idx];
                    break;
            }
        }

        public double GetLoss(int idx, int ant)
        {
            switch (ant)
            {
                case 0:
                    return data[tabControl.SelectedIndex][idx].Ant_0;
                case 1:
                    return data[tabControl.SelectedIndex][idx].Ant_1;
                case 2:
                    return data[tabControl.SelectedIndex][idx].Ant_2;
                case 3:
                    return data[tabControl.SelectedIndex][idx].Ant_3;
                case 4:
                    return data[tabControl.SelectedIndex][idx].Ant_4;
                case 5:
                    return data[tabControl.SelectedIndex][idx].Ant_5;
                case 6:
                    return data[tabControl.SelectedIndex][idx].Ant_6;
                case 7:
                    return data[tabControl.SelectedIndex][idx].Ant_7;
            }
            return 0;
        }
    }
}
