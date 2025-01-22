using System;

class Program
{
    public delegate void MyCallback(int result);

    static void MyFunction(int value, MyCallback callback)
    {
        Console.WriteLine($"Main logic with value: {value}");
        callback(value * 2);
    }

    static void MyCallbackFunction(int result)
    {
        Console.WriteLine($"Callback result: {result}");
    }

    static void Main()
    {
        MyFunction(10, MyCallbackFunction);
    }
}
