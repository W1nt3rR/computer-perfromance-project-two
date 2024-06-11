# Performance of computer systems

### This is a program designed to benchmark the performance of your processor, to show how multi threading can improve performance.

# Getting started

### To get started you can follow the instructions bellow to setup and compile the benchmark program.
The reason we compile the program on each computer is to make use of **-march=native -mtune=native** compiler flags to tune the program to the host processor to squeeze the most performance out of it.

# MSYS2 with Visual Studio Code

This guide will help you set up MSYS2 to work with Visual Studio Code for C/C++ development.

## Prerequisites

-   [Visual Studio Code](https://code.visualstudio.com/)
-   [MSYS2](https://www.msys2.org/)

## Installation Steps

### 1. Install MSYS2

1. Download the MSYS2 installer from the [official website](https://www.msys2.org/).
2. Run the installer and follow the instructions to install MSYS2.

### 2. Update MSYS2

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Update the package database and core system packages with the following command:

    ```sh
    pacman -Syu
    ```

### 3. Install Required Packages

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Install the development tools and other required packages. For example, to install the GNU Compiler Collection (GCC) and make:

    ```sh
    pacman -S base-devel gcc mingw-w64-x86_64-toolchain
    ```

### 4. Set Up Environment Variables.

1. Add the MSYS2 binary paths to your system PATH. Open your environment variables settings and add the following paths (assuming default installation directory):

    ```
    C:\msys64\usr\bin
    ```

    ```
    C:\msys64\mingw64\bin
    ```

### 5. Run with VS Code Debug

# Results:
Results generaly show a preference for higher thread count, though there are a few exceptions:

### 1. Core count limitation

Most modern processors have multiple threads per core (AMD SMT, Intel Hyper-Threading), meaning they can make the core execute two workloads simultaneously, provided they do not overlap in what part of the core they use. In the **EPYC Milan 32-core** test, some functions suffer from this when going to 64 threads from 32 threads on a 32-core processor, some other functions prefer it, although not by much.

### 2. Power limit

Laptop processors we tested on sometimes show preference of lower core count. That can be explained by power or temeprature limits. When pushed really hard as in this case, meaning we use multiple cores at once, they are forced to lower their core clocks to stay in their power and/or temeprature budget. That makes lower core count have higher core frequencies and thus executing the program threads faster. Multithreading overhead is way worse in this case.

# CPUs tested:

### 1. EPYC Milan 32-core

    Function        1 Thread     2 Threads    4 Threads    8 Threads    16 Threads   32 Threads   64 Threads   Best
       
    sumSquares      0.407453s    0.213925s    0.134193s    0.082021s    0.058176s    0.048877s    0.049932s    32 Threads
    step2           0.397043s    0.234918s    0.130374s    0.078339s    0.054715s    0.044696s    0.048838s    32 Threads
    quartic         2.76232s     1.98732s     1.319582s    0.9791243s   0.631817s    0.472643s    0.293942s    64 Threads
    powell          0.771935s    0.413548s    0.220815s    0.127074s    0.084019s    0.063775s    0.066261s    32 Threads
    rosenbrock      0.407224s    0.241453s    0.130955s    0.080175s    0.056469s    0.045330s    0.047915s    32 Threads
    dixonPrice      0.424583s    0.250595s    0.135528s    0.081505s    0.056557s    0.045338s    0.048679s    32 Threads
    schwefel2       0.717603s    0.407925s    0.217368s    0.123082s    0.081216s    0.062720s    0.068080s    32 Threads
    schwefel2_20    0.399818s    0.231633s    0.126215s    0.075794s    0.052911s    0.043569s    0.048174s    32 Threads
    schwefel2_21    0.384352s    0.227217s    0.126322s    0.075722s    0.051889s    0.043937s    0.047407s    32 Threads
    rastrigin       1.132815s    0.619458s    0.326188s    0.179962s    0.110913s    0.079300s    0.077352s    64 Threads
    griewank        0.998071s    0.543658s    0.286561s    0.161753s    0.104477s    0.076649s    0.081339s    32 Threads
    csendes         1.684713s    0.895438s    0.464978s    0.251801s    0.149661s    0.103815s    0.100267s    64 Threads
    colville        0.157882s    0.093666s    0.053550s    0.036285s    0.026613s    0.024452s    0.025047s    32 Threads
    easom           0.129255s    0.071381s    0.037713s    0.020694s    0.013159s    0.015083s    0.018910s    16 Threads
    michalewicz     0.279664s    0.173098s    0.098082s    0.057958s    0.036436s    0.029358s    0.028979s    64 Threads
    shekel          0.169479s    0.103513s    0.060968s    0.039185s    0.028733s    0.024380s    0.024821s    32 Threads
    schwefel2_4     0.412947s    0.242588s    0.134051s    0.080331s    0.056684s    0.047105s    0.049201s    32 Threads
    schwefel        0.829749s    0.463818s    0.248088s    0.142363s    0.094053s    0.071273s    0.076076s    32 Threads
    schaffer        1.133880s    0.615062s    0.322956s    0.109748s    0.113825s    0.082316s    0.081799s    64 Threads
    alpine          0.544085s    0.308934s    0.166355s    0.093357s    0.061646s    0.047163s    0.046618s    32 Threads
    ackley          0.656503s    0.373448s    0.198644s    0.111539s    0.070972s    0.051459s    0.050702s    64 Threads
    sphere          0.267508s    0.165615s    0.091066s    0.058244s    0.041963s    0.035389s    0.036648s    32 Threads
    schwefel2_22    0.271436s    0.166823s    0.093927s    0.059296s    0.042030s    0.035820s    0.036809s    32 Threads

### 2. Intel Core i5-7200U

    Function          1 Thread        2 Threads       4 Threads       Best
          
    sumSquares        0.556608s       0.352304s       0.268045s       4 Threads
    step2             0.546784s       0.330751s       0.264745s       4 Threads
    quartic           7.18188s        3.41311s        2.28181s        4 Threads
    powell            1.49732s        0.975036s       2.644436s       4 Threads
    rosenbrock        0.562728s       0.34109s        0.30655s        4 Threads
    dixonPrice        0.574671s       0.368834s       0.288526s       4 Threads
    schwefel1_2       1.248935s       0.758167s       0.562306s       4 Threads
    schwefel2_20      0.644795s       0.381267s       0.296368s       4 Threads
    schwefel2_21      0.638806s       0.357119s       0.282022s       4 Threads
    rastrigin         1.84971s        0.955761s       0.687356s       4 Threads
    griewank          1.7556s         1.043535s       0.844223s       4 Threads
    csendes           3.34133s        1.681745s       1.2984s         4 Threads
    colville          2.248418s       0.154443s       0.118036s       4 Threads
    easom             0.180859s       0.12652s        0.107966s       4 Threads
    michalewicz       0.619675s       0.360739s       0.223255s       4 Threads
    shekel            0.286889s       0.196228s       0.207659s       4 Threads
    schwefel2_4       0.886635s       0.436185s       0.429423s       4 Threads
    schwefel          1.60399s        0.944004s       0.654302s       4 Threads
    schaffer          1.0092s         0.05938s        0.7434s         4 Threads
    alpine            0.927964s       0.615063s       0.507563s       4 Threads
    ackley            1.13396s        0.584685s       0.419165s       4 Threads
    sphere            0.400215s       0.252536s       0.205031s       4 Threads
    schwefel2_22      0.363431s       0.234626s       0.195932s       4 Threads

### 3. AMD Ryzen 7 5825U

    Function          1 Thread        2 Threads       4 Threads       8 Threads       16 Threads      Best

    sumSquares        0.225248s       0.127604s       0.0832489s      0.067521s       0.0753114s      8 Threads
    step2             0.21375s        0.122257s       0.0795047s      0.0681404s      0.0733422s      8 Threads
    quartic           2.7722s         2.15209s        1.26891s        0.819747s       0.55904s        16 Threads
    powell            0.27365s        0.250231s       0.131094s       0.0926374s      0.0975484s      8 Threads
    rosenbrock        0.218611s       0.185577s       0.108691s       0.084943s       0.0824957s      16 Threads
    dixonPrice        0.243443s       0.129654s       0.0850587s      0.0682017s      0.0752841s      8 Threads
    schwefel1_2       0.455494s       0.248667s       0.155018s       0.112533s       0.114393s       8 Threads
    schwefel2_20      0.209815s       0.120883s       0.0822827s      0.0664947s      0.0731139s      8 Threads
    schwefel2_21      0.20433s        0.117634s       0.0779667s      0.0637488s      0.0721856s      8 Threads
    rastrigin         1.56259s        0.820379s       0.474328s       0.305822s       0.255712s       16 Threads
    griewank          1.64756s        0.869126s       0.50872s        0.330612s       0.286489s       16 Threads
    csendes           3.04121s        1.59425s        0.940341s       0.604614s       0.477266s       16 Threads
    colville          0.0814221s      0.051607s       0.0368126s      0.0335543s      0.0473146s      8 Threads
    easom             0.0390843s      0.0285907s      0.0231728s      0.0256369s      0.0421327s      4 Threads
    michalewicz       0.37284s        0.208144s       0.121516s       0.0891876s      0.0874603s      16 Threads
    shekel            0.0904187s      0.0561062s      0.0388413s      0.0347363s      0.0470084s      8 Threads
    schwefel2_4       0.224707s       0.12505s        0.0816654s      0.0662279s      0.0746262s      8 Threads
    schwefel          1.57084s        0.82027s        0.479785s       0.323849s       0.272601s       16 Threads
    schaffer          1.65047s        0.868561s       0.503404s       0.323251s       0.259863s       16 Threads
    alpine            0.809012s       0.428849s       0.251862s       0.171211s       0.150482s       16 Threads
    ackley            0.848329s       0.451769s       0.262129s       0.174342s       0.156157s       16 Threads
    sphere            0.153317s       0.0898572s      0.0596597s      0.0501195s      0.0602818s      8 Threads
    schwefel2_22      0.126089s       0.0760976s      0.0521025s      0.0449394s      0.0571523s      8 Threads

### 4. AMD Ryzen 7 5700U

    Function          1 Thread        2 Threads       4 Threads       8 Threads       16 Threads      Best

    sumSquares        0.345286s       0.224498s       0.127089s       0.100642s       0.130611s       8 Threads
    step2             0.339045s       0.239902s       0.142093s       0.105901s       0.135295s       8 Threads
    quartic           2.615455s       1.451415s       0.801285s       0.552465s       0.534865s       16 Threads
    powell            1.497325s       0.449047s       0.243945s       0.197269s       0.203645s       8 Threads
    rosenbrock        0.350949s       0.227965s       0.127825s       0.101459s       0.131079s       8 Threads
    dixonPrice        0.365409s       0.232467s       0.132843s       0.103795s       0.132681s       8 Threads
    schwefel1_2       0.681945s       0.411349s       0.268535s       0.162921s       0.168245s       8 Threads
    schwefel2_20      0.346002s       0.221755s       0.124891s       0.101995s       0.127255s       8 Threads
    schwefel2_21      0.343645s       0.217271s       0.125019s       0.105612s       0.127576s       8 Threads
    rastrigin         1.027775s       0.619597s       0.335485s       0.286445s       0.216374s       16 Threads
    griewank          1.024650s       0.622355s       0.376275s       0.265268s       0.249529s       16 Threads
    csendes           1.705550s       1.134845s       0.692532s       0.393582s       0.300335s       16 Threads
    colville          0.148621s       0.136572s       0.086815s       0.078892s       0.137315s       8 Threads
    easom             0.126429s       0.125350s       0.074348s       0.079750s       0.143687s       4 Threads
    michalewicz       0.281123s       0.222835s       0.134247s       0.099884s       0.134847s       8 Threads
    shekel            0.157489s       0.140369s       0.093323s       0.080929s       0.134942s       8 Threads
    schwefel2_4       0.362398s       0.300908s       0.176592s       0.125126s       0.135925s       8 Threads
    schwefel          0.880865s       0.638475s       0.371595s       0.231085s       0.200074s       16 Threads
    schaffer          1.082435s       0.734115s       0.429430s       0.261655s       0.215215s       16 Threads
    alpine            0.496483s       0.378628s       0.210855s       0.145486s       0.147458s       8 Threads
    ackley            0.607943s       0.440826s       0.262575s       0.162955s       0.154589s       16 Threads
    sphere            0.237115s       0.206815s       0.121635s       0.095666s       0.139325s       8 Threads
    schwefel2_22      0.243965s       0.209034s       0.123953s       0.095657s       0.135263s       8 Threads
