# Compute shader for pratice use

**Ver. UE 5.0**

# 1. What it dose? 
        1. Pass a RT into CS1;
        2. CS1 do stuff wrote in ComputeShaderTest.usf;
        3. output a UAV 'MyTex';
        4. Pass MyTex into CS2;
        5. CS2 do stuff wrote in ComputeShaderTest2.usf;
        6. output a UAV 'MyTex2';
        7. copy MyTex2 to RT;

# 2.TODO 
|Content | At Pos|
|---|---|
|//TODO Maybe a issue cause copy RT right after RDG exectue, and CS may not finish at this timing,may be have a delegate? | Source\ComputeShaderTestPlugin\Public\ComputeShaderTest.cpp:104|
