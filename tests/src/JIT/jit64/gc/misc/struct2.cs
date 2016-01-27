// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//

using System;

struct S
{
    public String str;
}

class Test
{
    public static void c(S s1, S s2)
    {
        Console.WriteLine(s1.str + s2.str);
    }

    public static int Main()
    {
        S sM, sM2;

        sM.str = "test";
        sM2.str = "test2";
        c(sM, sM2);
        return 100;
    }
}
