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
using System.Windows.Shapes;

namespace FacilityCableTool
{
    /// <summary>
    /// InputWindow.xaml 的交互逻辑
    /// </summary>
    public partial class InputWindow : Window
    {
        public double minfre { get; set; }
        public double maxfre { get; set; }
        public double freqval { get; set; }
        public bool addresult { get; set; }
        public InputWindow()
        {
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            InitializeComponent();
        }

        private void textBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            if (textBox.Text.Length != 0)
            {
                var val = Convert.ToDouble(textBox.Text);
                if (val < minfre || val > maxfre)
                {
                    MessageBoxResult result = MessageBox.Show("所设置的频偏值已超出对应频段的范围，请重新设置。", "警告", MessageBoxButton.OK);
                }
                else
                {
                    this.Close();
                    addresult = true;
                    freqval = val;
                }
            }
            else
            {
                addresult = false;
                this.Close();
            }
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
            addresult = false;
        }

        private void keyDown(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                if (textBox.Text.Length != 0)
                {
                    var val = Convert.ToDouble(textBox.Text);
                    if (val < minfre || val > maxfre)
                    {
                        MessageBoxResult result = MessageBox.Show(String.Format("所设置的频偏值已超出对应频段的范围({0}, {1})，请重新设置。", minfre.ToString(), maxfre.ToString(), "警告", MessageBoxButton.OK));
                    }
                    else
                    {
                        this.Close();
                        addresult = true;
                        freqval = val;
                    }
                }
                else
                {
                    addresult = false;
                    this.Close();
                }
            }
        }
    }
    }
