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
using System.Diagnostics;

using ParetoCalculatorUI.Utils;
using System.Collections;
using System.IO;
using System.Reflection;

namespace ParetoCalculatorUI.Dialogs
{
    /// <summary>
    /// Interaction logic for GraphDialog.xaml
    /// </summary>
    public partial class GraphDialog : Window
    {

        private readonly ParetoCalculatorW my_pc;

        public GraphDialog(ParetoCalculatorW pc)
        {
            InitializeComponent();
            this.my_pc = pc;
            ArrayList quants = this.my_pc.confsetConfspaceQuantityNames();
            foreach (String s in quants)
            {
                this.HorizontalCombo.Items.Add(s);
                this.VerticalCombo.Items.Add(s);
            }
        }

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            this.Close();
        }

        private void PlotButton_Click(object sender, RoutedEventArgs e)
        {
            //compute points
            string qa = this.HorizontalCombo.Text;
            string qb = this.VerticalCombo.Text;
            ArrayList points = this.my_pc.getScattterPoints(qa, qb);
            Charts.ShowScatterChart(qa, qb, points);
        }

        private void QuantityChanged()
        {
            if (this.HorizontalCombo.SelectedIndex >= 0 && this.VerticalCombo.SelectedIndex >= 0)
            {
                string qx = (string)this.HorizontalCombo.SelectedItem;
                string qy = (string)this.VerticalCombo.SelectedItem;
                ArrayList points = this.my_pc.getScattterPoints(qx, qy);

                using (StringWriter strWriter = new StringWriter())
                {
                    strWriter.Write("{");

                    List<string> pointData = new List<string>(); ;
                    foreach (ArrayList l in points)
                    {
                        pointData.Add(String.Format("({0}, {1})", l[0], l[1]));
                    }

                    strWriter.Write(String.Join(", ", pointData));
                    strWriter.Write("}");

                    this.ChartText.Text = strWriter.ToString();
                }
            }
        }

        private void HorizontalCombo_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            this.QuantityChanged();
        }

        private void VerticalCombo_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            this.QuantityChanged();
        }
    }
}
