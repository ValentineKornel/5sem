using System;
using System.Collections.Generic;
using System.Threading;

class Program
{
    static void Main(string[] args)
    {
        List<byte[]> memoryChunks = new List<byte[]>();
        Console.WriteLine("Приложение OS08_02: создание объекта 128 МБ каждые 5 секунд.");

        while (true)
        {
            try
            {
                byte[] chunk = new byte[128 * 1024 * 1024];
                memoryChunks.Add(chunk);

                Thread fillThread = new Thread(() => FillMemory(chunk));
                fillThread.Start();
                fillThread.Join();

                long usedMemory = GC.GetTotalMemory(false);
                Console.WriteLine($"Использовано памяти: {usedMemory / 1024 / 1024} МБ");
            }
            catch (OutOfMemoryException)
            {
                Console.WriteLine("Недостаточно памяти для создания нового объекта.");
                break;
            }

            Thread.Sleep(5000);
        }
    }

    static void FillMemory(byte[] chunk)
    {
        for (int i = 0; i < chunk.Length; i += 1024)
        {
            chunk[i] = 1;
        }
    }
}
