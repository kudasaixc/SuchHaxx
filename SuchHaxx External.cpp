        
        
                                                     //////////////////////////////////////////////////////////////
                                                    //                  * XC TEAM *                             //
                                                   //   This source code, made by Kudasai (XC) is              //
                                                  //    licensed under a Creative Commons license             //
                                                 //     (CC BY-NC-SA 4.0). You may find the license          // 
                                                //                        here:                             //
                                               //  https://creativecommons.org/licenses/by-nc-sa/4.0/      //
                                              //        We hope you enjoy this source code! kekw          //
                                             //         Goal: Edit Such Art money pointer in order       //
                                            //          to grant user ability to edit their balance.    //
                                           //////////////////////////////////////////////////////////////
                                          //                                                          //
                                         //  NOTE: I was really lazy to comment all the code so yeah //
                                        //     							    //          
                                       //////////////////////////////////////////////////////////////



#include <iostream>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <cctype>
#include <type_traits>
#include <chrono>
#include <thread>

template <auto t>
using constant_t = std::integral_constant<decltype(t), t>;

using smart_handle = std::unique_ptr<void, constant_t<CloseHandle>>; // a type for managing handles (automatically frees them when they go out of scope)

std::uint32_t get_process_pid(std::string name)
{
    auto snapshot = smart_handle{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) }; // create a snapshot of all the processes
	
    PROCESSENTRY32 entry{ 0 };
    entry.dwSize = sizeof(PROCESSENTRY32); // set the size of the structure

    Process32First(snapshot.get(), &entry); // get the first process

    do
    { // we won't do == here because it's a pointer
        if (!strcmp(entry.szExeFile, name.c_str())) // if the process name matches the name we're looking for
            return entry.th32ProcessID; // return the PID
		
    } while (Process32Next(snapshot.get(), &entry)); // iterate the processes

    return 0x0; // if we get here, we didn't find the process
} 

std::uintptr_t get_process_module(std::uint32_t pid, std::string name)
{
    auto snapshot = smart_handle{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid) }; // create a snapshot of all the modules

    MODULEENTRY32 entry{ 0 };
    entry.dwSize = sizeof(MODULEENTRY32);

    Module32First(snapshot.get(), &entry);

    do
    {
        if (!strcmp(entry.szModule, name.c_str()))
            return reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);

    } while (Module32Next(snapshot.get(), &entry));

    return 0x0;
}


std::uintptr_t follow_pointer_path(HANDLE handle, std::uintptr_t base, std::vector<std::uint32_t> offsets) 
{
    std::uintptr_t addr = base; 
	
    ReadProcessMemory(handle, reinterpret_cast<void*>(addr), &addr, sizeof(addr), nullptr); 
	
    for (auto i = 0u; i < offsets.size() - 1; i++) 
        ReadProcessMemory(handle, reinterpret_cast<void*>(addr + offsets[i]), &addr, sizeof(addr), nullptr); 

    return addr + offsets[offsets.size() - 1]; 
} 

int main() 
{
    using namespace std::this_thread;     
    using namespace std::chrono_literals; 

#define nwln "\n"
    std::cout << "Initializing SuchHaxx Revision 2" << nwln;
    sleep_for(800ms);
    auto proc_pid = get_process_pid("SuchArt.exe");
	auto proc_handle = smart_handle{ OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_pid) }; 
    auto mono_bdwgc = get_process_module(proc_pid, "mono-2.0-bdwgc.dll"); // works
    
    
    std::cout << std::hex << "MONO Offset : 0x" << std::uppercase << mono_bdwgc << std::dec << nwln;
    sleep_for(100ms);
    auto money_addr = follow_pointer_path(proc_handle.get(), mono_bdwgc + 0x004A1578, { 0x630, 0x20, 0x270, 0x110, 0x78, 0x90, 0x98 }); // 
    sleep_for(150ms);
    std::cout << "Money offset : 0x" << std::hex << std::uppercase << money_addr << std::dec << nwln;
    sleep_for(300ms);
    
        while (true) {
            system("cls");
            std::cout << "How much money would you like to add?\n";
            std::uint64_t money{ 0 };
            std::cin >> money;
            WriteProcessMemory(proc_handle.get(), reinterpret_cast<void*>(money_addr), &money, sizeof(money), nullptr);
        }
}

        
        
                                                     //////////////////////////////////////////////////////////////
                                                    //                  * XC TEAM *                             //
                                                   //   This source code, made by Kudasai (XC) is              //
                                                  //    licensed under a Creative Commons license             //
                                                 //     (CC BY-NC-SA 4.0). You may find the license          // 
                                                //                        here:                             //
                                               //  https://creativecommons.org/licenses/by-nc-sa/4.0/      //
                                              //        We hope you enjoy this source code! kekw          //
                                             //         Goal: Edit Such Art money pointer in order       //
                                            //          to grant user ability to edit their balance.    //
                                           //////////////////////////////////////////////////////////////
                                          //                                                          //
                                         //  NOTE: I was really lazy to comment all the code so yeah //
                                        //     							    //          
                                       //////////////////////////////////////////////////////////////


