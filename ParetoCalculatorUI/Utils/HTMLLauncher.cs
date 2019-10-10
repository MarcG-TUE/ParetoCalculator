using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
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
}
