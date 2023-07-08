#include "shell.hh"
#include <chrono>

int main()
{ std::string input;

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = "***************\n***HOOFDMENU***\n***************\n Prompt: ";

  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file()
{
    std::string fileName = "";
    std::string tekst = "";
    std::string appendTekst = "";

    std::cout << "Hoe wil je het bestand noemen?" << std::endl;
    std::getline(std::cin, fileName);
    const char* name_array[] = {NULL};
    name_array[0] = fileName.c_str();
    
    int fd = syscall(SYS_creat, name_array[0], 0644);

    std::cout << "Wat wil je erin schrijven?" << std::endl;
    while(std::getline(std::cin, tekst)){
        if (tekst == "<EOF>"){
            break;
        }
        else{
            appendTekst += tekst + "\n";
        }
    }
    const char* tekst_array[] = {NULL};
    tekst_array[0] = appendTekst.c_str();

    syscall(SYS_write, fd, tekst_array[0],appendTekst.size());
    
}

void list()
{
    int pid = syscall(SYS_fork);
    if (pid == 0){
        const char* argumenten[] = {"/bin/ls", "-l", "-a", NULL};
        syscall(SYS_execve, argumenten[0], argumenten, NULL );
    }
    else{
        syscall(SYS_wait4, pid, NULL, NULL, NULL);
    }
}

void find()
{
    std::string str;
    std::cout << "Waar wil je op zoeken?" << std::endl;
    getline(std::cin, str);
    int fd[2];
    syscall(SYS_pipe, fd);

    int pidF = syscall(SYS_fork);

    if (pidF == 0){
        const char* argsF[] = {"/usr/bin/find", ".", NULL};
        syscall(SYS_dup2, fd[1], 1);
        syscall(SYS_execve, argsF[0], argsF, NULL);
    }
    
    int pidG = syscall(SYS_fork);

    if (pidG == 0){
        syscall(SYS_close, fd[1]);
        const char* argsG[] ={"/bin/grep", str.c_str(), NULL};
        syscall(SYS_dup2, fd[0], 0);
        syscall(SYS_execve, argsG[0], argsG, NULL);
    }

    syscall(SYS_close, fd[0]);
    syscall(SYS_close, fd[1]);

    syscall(SYS_wait4, pidF, NULL, NULL, NULL);
    syscall(SYS_wait4, pidG, NULL, NULL, NULL);

}

void seek()
{
    int fdS = syscall(SYS_creat, "seek", 0644);
    int fdL = syscall(SYS_creat, "loop", 0644);

    auto tS1 = std::chrono::high_resolution_clock::now();
    syscall(SYS_write, fdS, "x", 1);
    syscall(SYS_lseek, fdS, 5000000, SEEK_END);
    syscall(SYS_write, fdS, "x", 1);
    auto tS2 = std::chrono::high_resolution_clock::now();

    auto tL1 = std::chrono::high_resolution_clock::now();
    syscall(SYS_write, fdL, "x", 1);
    for (int i = 0; i < 5000000; i++)
    {
        syscall(SYS_write, fdL, "\0", 1);
    }
    syscall(SYS_write, fdL, "x", 1);
    auto tL2 = std::chrono::high_resolution_clock::now();

    syscall(SYS_close, fdS);
    syscall(SYS_close, fdL);

    auto Sms = std::chrono::duration_cast<std::chrono::nanoseconds>(tS2 -tS1);
    auto Lms = std::chrono::duration_cast<std::chrono::milliseconds>(tL2 -tL1);
    std::cout << "Seek took " << Sms.count() << " nanoseconds." << std::endl;
    std::cout << "Loop took " << Lms.count() << " ms." << std::endl;

}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
