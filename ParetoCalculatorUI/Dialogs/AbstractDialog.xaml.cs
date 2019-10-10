using System;
using System.Collections;
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

namespace ParetoCalculatorUI.Dialogs
{
    /// <summary>
    /// Interaction logic for AbstractDialog.xaml
    /// </summary>
    public partial class AbstractDialog : Window
    {
        public AbstractDialog(ParetoCalculatorW pc)
        {
            InitializeComponent();
            ArrayList quants = pc.confsetConfspaceQuantityNames();
            foreach (String s in quants)
            {
                this.abstractCombo.Items.Add(s);
            }
        }

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            this.Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
            this.Close();
        }
    }
}
