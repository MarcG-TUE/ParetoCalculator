using System;
using System.Collections.Generic;
using System.Collections;
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

namespace ParetoCalculatorUI.Dialogs
{
    /// <summary>
    /// Interaction logic for JoinDialog.xaml
    /// </summary>
    public partial class JoinDialog : Window
    {
        public JoinDialog(ParetoCalculatorW pc)
        {
            InitializeComponent();
            ArrayList quants = pc.confsetConfspaceQuantityNames();
            foreach (String s in quants)
            {
                this.quantCombo.Items.Add(s);
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
    }
}
