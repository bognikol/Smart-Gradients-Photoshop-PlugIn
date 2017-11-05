using System;
using System.Collections.Generic;
using System.Reflection;
using System.IO;
using System.Diagnostics;

namespace SmartGradientsInstaller
{
    public class PhotoshopInstallationInstance
    {
        public PhotoshopInstallationInstance(String bits, String version, String path)
        {
            Path = path;
            Version = version;
            Bits = bits;
        }

        public Boolean IncludeInInstall = true;
        public Boolean? InstallationSuccesfull = null;
        public Exception ExceptionIfFailure = null;
        public String Path;
        private String _Version;
        public String Version
        {

            get { return _Version; }
            set
            {
                if (value.Contains("14.0") || value.Contains("14.1") || value.Contains("14.2"))
                {
                    _Version = "CC";
                }
                else if (value.Contains("15.0"))
                {
                    _Version = "CC 2014";
                }
                else if (value.Contains("16.0"))
                {
                    _Version = "CC 2015";
                }
                else
                {
                    _Version = value;
                }
            }

        }
        public Boolean Is64BitPhotoshop()
        {
            if (Bits == "64bit") return true;
            return false;
        }
        public String Bits;
        public String GetFullName()
        {
            return "Photoshop " + Version + " (" + Bits + ")";
        }

    }

    static class MainRoutines
    {
        static public List<PhotoshopInstallationInstance> GetCandidates(bool is64bitWindows)
        {
            List<PhotoshopInstallationInstance> candidates = new List<PhotoshopInstallationInstance>();

            PhotoshopInstallationInstance currentInstance;
            if (is64bitWindows)
            {
                String[] files = Directory.GetDirectories(Environment.GetEnvironmentVariable("ProgramFiles(x86)"));
                foreach (String s in files)
                {
                    if (s.Contains("Adobe"))
                    {
                        String[] innerfiles = Directory.GetDirectories(s);
                        foreach (String ss in innerfiles)
                        {
                            if (ss.Contains("Photoshop"))
                            {
                                currentInstance = IsPhotoshopFolder(ss);
                                if (currentInstance != null)
                                {
                                    candidates.Add(currentInstance);
                                }
                            }
                        }
                    }
                    else if (s.Contains("Photoshop"))
                    {
                        currentInstance = IsPhotoshopFolder(s);
                        if (currentInstance != null)
                        {
                            candidates.Add(currentInstance);
                        }
                    }
                }
                files = Directory.GetDirectories(Environment.GetEnvironmentVariable("ProgramFiles"));
                foreach (String s in files)
                {
                    if (s.Contains("Adobe"))
                    {
                        String[] innerfiles = Directory.GetDirectories(s);
                        foreach (String ss in innerfiles)
                        {
                            if (ss.Contains("Photoshop"))
                            {
                                currentInstance = IsPhotoshopFolder(ss);
                                if (currentInstance != null)
                                {
                                    candidates.Add(currentInstance);
                                }
                            }
                        }
                    }
                    else if (s.Contains("Photoshop"))
                    {
                        currentInstance = IsPhotoshopFolder(s);
                        if (currentInstance != null)
                        {
                            candidates.Add(currentInstance);
                        }
                    }
                }
            }
            else
            {
                String[] files = Directory.GetDirectories(Environment.GetEnvironmentVariable("ProgramFiles"));
                foreach (String s in files)
                {
                    if (s.Contains("Adobe"))
                    {
                        String[] innerfiles = Directory.GetDirectories(s);
                        foreach (String ss in innerfiles)
                        {
                            if (ss.Contains("Photoshop"))
                            {
                                currentInstance = IsPhotoshopFolder(ss);
                                if (currentInstance != null)
                                {
                                    candidates.Add(currentInstance);
                                }
                            }
                        }
                    }
                    else if (s.Contains("Photoshop"))
                    {
                        currentInstance = IsPhotoshopFolder(s);
                        if (currentInstance != null)
                        {
                            candidates.Add(currentInstance);
                        }
                    }
                }
            }
            return candidates;
        }

        static private String tempPath;

        static public bool InstallCppRedistributableV140(bool is64bitWindows, EventHandler handler)
        {
            try
            {
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();

                tempPath = Path.GetTempPath();

                if (is64bitWindows)
                {
                    tempPath += "/phtshpPlgNSmrtGrdntsCppRntTempvcredist_x64.exe";
                    WriteResourceToFile("SmartGradientsInstaller.v140.vcredist_x64.exe", tempPath);

                }
                else
                {
                    tempPath += "/phtshpPlgNSmrtGrdntsCppRntTempvcredist_x86.exe";
                    WriteResourceToFile("SmartGradientsInstaller.v140.vcredist_x86.exe", tempPath);
                }

                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.FileName = "cmd.exe";
                startInfo.Arguments = "/c " + tempPath + " /q /norestart";
                process.StartInfo = startInfo;
                process.EnableRaisingEvents = true;
                process.Exited += handler;

                process.Start();

                return true;
            }
            catch (Exception)
            {
                return false;
            }

        }

        static public bool InstallPlugIn(String destination, bool is64bitPhotoshop)
        {
            try
            {
                if (is64bitPhotoshop)
                {
                    WriteResourceToFile("SmartGradientsInstaller.x86_64.SmartGradients.8bf",  destination + "/Plug-ins/SmartGradients.8bf");
                    WriteResourceToFile("SmartGradientsInstaller.x86_64.GradientControl.dll", destination + "/GradientControl.dll");
                }
                else
                {
                    WriteResourceToFile("SmartGradientsInstaller.x86_32.SmartGradients.8bf",  destination + "/Plug-ins/SmartGradients.8bf");
                    WriteResourceToFile("SmartGradientsInstaller.x86_32.GradientControl.dll", destination + "/GradientControl.dll");
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }

        }

        public static PhotoshopInstallationInstance IsPhotoshopFolder(String str)
        {
            try
            {
                bool containsApp = false;
                bool containsPluginsDir = false;
                String version = "";
                String bitsFlag = "";

                String[] files = Directory.GetFiles(str);
                foreach (String f in files)
                {
                    if (f.Contains("Photoshop.exe"))
                    {
                        FileVersionInfo fvi = FileVersionInfo.GetVersionInfo(f);
                        version = fvi.ProductVersion;
                        bitsFlag = GetBitsFlag(f);
                        containsApp = true;
                        break;
                    }
                }

                files = Directory.GetDirectories(str);
                foreach (String f in files)
                {
                    if (f.Contains("Plug-ins"))
                    {
                        containsPluginsDir = true;
                        break;
                    }
                }

                if (containsApp && containsPluginsDir)
                {
                    return new PhotoshopInstallationInstance(bitsFlag, version, str);
                }
                return null;
            }
            catch (Exception)
            {
                return null;
            }
        }

        private static void WriteResourceToFile(String resourceName, String fileName)
        {
            using (Stream resource = Assembly.GetExecutingAssembly().GetManifestResourceStream(resourceName))
            {
                using (FileStream file = new FileStream(fileName, FileMode.Create, FileAccess.Write))
                {
                    resource.CopyTo(file);
                }
            }
        }

        private enum MachineType
        {
            Native  = 0,
            I386    = 0x014c,
            Itanium = 0x0200,
            x64     = 0x8664
        }

        private static MachineType GetMachineType(String executablePath)
        {
            const Int32 PE_POINTER_OFFSET = 60;
            const Int32 MACHINE_OFFSET = 4;
            Byte[] data = new Byte[4096];
            using (Stream s = new FileStream(executablePath, FileMode.Open, FileAccess.Read))
            {
                s.Read(data, 0, 4096);
            }
            // dos header is 64 bytes, last element, long (4 bytes) is the address of the PE header
            Int32 PE_HEADER_ADDR = BitConverter.ToInt32(data, PE_POINTER_OFFSET);
            Int32 machineUint = BitConverter.ToUInt16(data, PE_HEADER_ADDR + MACHINE_OFFSET);
            return (MachineType)machineUint;
        }

        private static String GetBitsFlag(String executablePath)
        {
            String bits = "";
            MachineType t = GetMachineType(executablePath);

            if (t == MachineType.I386)
            {
                bits = "32bit";
            }
            else if (t == MachineType.x64)
            {
                bits = "64bit";
            }
            return bits;
        }
    }
}
