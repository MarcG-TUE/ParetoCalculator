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

namespace ParetoCalculatorUI.Dialogs
{
    /// <summary>
    /// Interaction logic for GraphDialog.xaml
    /// </summary>
    public partial class GraphDialog : Window
    {

        private ParetoCalculatorW my_pc;

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

        private void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            this.Close();
        }

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
            this.Close();
        }

        private void plotButton_Click(object sender, RoutedEventArgs e)
        {
            //compute points
            ArrayList points = this.my_pc.getScattterPoints(this.HorizontalCombo.Text, this.VerticalCombo.Text);

            StringWriter strWriter = new StringWriter();
            strWriter.Write("data: [\n");

            ArrayList pointData = new ArrayList();
            foreach (ArrayList l in points)
            {
                pointData.Add(String.Format("{ x: {0}, y: {1} }\n", l[0], l[1]);
            }

            strWriter.Write(String.Join(", ", pointData));
            strWriter.Write("]");

            string chartData = strWriter.ToString();

            string templateDoc = ReadFile("charttemplate.html");
            string chartDoc = templateDoc.Replace("#chartdata#", chartData);
            WriteFile("chart.html", chartDoc);

            EdgeLauncher.Launch(@"file:///C:\Users\mgeil\Documents\Software\ParetoCalculator\html\chart.html");
            //Process.Start(@"microsoft -edge:file:///C:\Users\mgeil\Documents\Software\ParetoCalculator\html\chart.html"); 
        }
    }
}
