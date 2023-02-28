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

    /// <summary>
    /// The HTMLLauncher class hosts any function related to launching a web browser.
    /// </summary>
    class HTMLLauncher
    {

        public static string Test
        {
            get
            {
                return "";
            }
        }

        /// <summary>
        /// Get the path to the current default web browser application. 
        /// </summary>
        /// <returns>Path to the current default web browser application as a string, or null if it could not be established.</returns>
        public static string DefaultWebBrowser
        {
            get
            {
                // some solution found on stackoverflow...
                // https://stackoverflow.com/questions/13621467/how-to-find-default-web-browser-using-c/17599201

                const string userChoice = @"Software\Microsoft\Windows\Shell\Associations\UrlAssociations\http\UserChoice";
                string progId;
                using (RegistryKey userChoiceKey = Registry.CurrentUser.OpenSubKey(userChoice))
                {
                    if (userChoiceKey == null) return null;
                    object progIdValue = userChoiceKey.GetValue("Progid");
                    if (progIdValue == null) return null;
                    progId = progIdValue.ToString();
                }
                const string exeSuffix = ".exe";
                string path = progId + @"\shell\open\command";
                using (RegistryKey pathKey = Registry.ClassesRoot.OpenSubKey(path))
                {
                    if (pathKey == null) return null;

                    // Trim parameters.
                    try
                    {
                        path = pathKey.GetValue(null).ToString().ToLower().Replace("\"", "");
                        if (!path.EndsWith(exeSuffix))
                        {
                            path = path.Substring(0, path.LastIndexOf(exeSuffix, StringComparison.Ordinal) + exeSuffix.Length);
                            return path;
                        }
                        return null;
                    }
                    catch
                    {
                        // Assume the registry value is set incorrectly, or some funky browser is used which currently is unknown.
                        return null;
                    }
                }
            }
        }
    }

    /// <summary>
    /// The Charts class provides functions to display data as charts.
    /// </summary>
    class Charts
    {
        // template html file for the scatter chart
        private const string scatterTemplate = @"html\charttemplate.html";

        /// <summary>
        /// Display x-y data as a sctter chart.
        /// </summary>
        /// <param name="qx">Name of the x-axis quantity.</param>
        /// <param name="qy">Name of the y-axis quantity.</param>
        /// <param name="points">ArrayList of ArrayLists with x and y data to show.</param>
        public static void ShowScatterChart(string qx, string qy, ArrayList points)
        {
            // make data representation for charts.js
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

            // fill data in html tempalte
            string templateDoc = File.ReadAllText(Charts.scatterTemplate);
            string labelStr = String.Format("label: '{0} vs. {1}'", qx, qy);
            string chartDoc = templateDoc.Replace("data: template", chartData).Replace("label: label", labelStr);

            // write html file
            File.WriteAllText(@"html\chart.html", chartDoc);
            string pwd = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            string chartfullpath = String.Format(@"""{0}\html\chart.html""", pwd);

            // get the default web browser on the system
            string browser = HTMLLauncher.DefaultWebBrowser;
            if (browser == null)
            {
                throw new ParetoCalculatorUIException("Cannot launch web browser.");
            }
            // open the web browser with the chart
            System.Diagnostics.Process.Start(browser, chartfullpath);
        }
    }

}
