using Microsoft.Win32;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace ParetoCalculatorUI.Utils
{

    class HTMLLauncher
    {

        public static string DefaultWebBrowser()
        {
            const string userChoice = @"Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http\UserChoice";
            string progId;
            using (RegistryKey userChoiceKey = Registry.CurrentUser.OpenSubKey(userChoice))
            {
                if (userChoiceKey == null)
                {
                    return "";
                }
                object progIdValue = userChoiceKey.GetValue("Progid");
                if (progIdValue == null)
                {
                    return "";
                }
                progId = progIdValue.ToString();
            }
            const string exeSuffix = ".exe";
            string path = progId + @"\shell\open\command";
            using (RegistryKey pathKey = Registry.ClassesRoot.OpenSubKey(path))
            {
                if (pathKey == null)
                {
                    return "";
                }

                // Trim parameters.
                try
                {
                    path = pathKey.GetValue(null).ToString().ToLower().Replace("\"", "");
                    if (!path.EndsWith(exeSuffix))
                    {
                        path = path.Substring(0, path.LastIndexOf(exeSuffix, StringComparison.Ordinal) + exeSuffix.Length);
                        return path;
                    }
                    return "";
                }
                catch
                {
                    // Assume the registry value is set incorrectly, or some funky browser is used which currently is unknown.
                    return "";
                }
            }
        }

    }

    class Charts
    {
        private const string scatterTemplate = @"html\charttemplate.html";

        public static void ShowScatterChart(string qa, string qb, ArrayList points)
        {
            string chartData;
            using (StringWriter strWriter = new StringWriter())
            {
                strWriter.Write("data: [\n");

                List<string> pointData = new List<string>();
                foreach (ArrayList l in points)
                {
                    pointData.Add(String.Format("{{ x: {0}, y: {1} }}\n", l[0], l[1]));
                }

                strWriter.Write(String.Join(", ", pointData));
                strWriter.Write("]");

                chartData = strWriter.ToString();
            }

            string templateDoc = File.ReadAllText(Charts.scatterTemplate);
            string labelStr = String.Format("label: '{0} vs. {1}'", qa, qb);
            string chartDoc = templateDoc.Replace("data: template", chartData).Replace("label: label", labelStr);

            File.WriteAllText(@"html\chart.html", chartDoc);
            string pwd = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            string chartfullpath = String.Format(@"""{0}\html\chart.html""", pwd);
            string browser = HTMLLauncher.DefaultWebBrowser();
            System.Diagnostics.Process.Start(browser, chartfullpath);
        }
    }

}
