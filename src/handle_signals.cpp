//
// Created by vaibz on 26/08/25.
//

#include <csignal>
#include "../include/handle_signals.h"
#include "../include/log.h"

void init_signal_handler()
{
    // registering signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGBUS, signal_handler);
    signal(SIGPIPE, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGQUIT, signal_handler);
}

void signal_handler(int signum)
{
    switch (signum)
    {
        case SIGINT:
            log(Log::INFO, "", "Received SIGINT (Interrupt from keyboard: Ctrl+C). Server terminated.");
            break;

        case SIGTERM:
            log(Log::INFO, "", "Received SIGTERM (Termination request, e.g. 'kill' command). Server terminated.");
            break;

        case SIGABRT:
            log(Log::INFO, "", "Received SIGABRT (Process aborted via abort()). Server terminated.");
            break;

        case SIGFPE:
            log(Log::INFO, "", "Received SIGFPE (Erroneous arithmetic operation: divide by zero, overflow, etc.). Server terminated.");
            break;

        case SIGSEGV:
            log(Log::INFO, "", "Received SIGSEGV (Invalid memory access: segmentation fault). Server terminated.");
            break;

        case SIGILL:
            log(Log::INFO, "", "Received SIGILL (Illegal instruction: corrupted code or bad jump). Server terminated.");
            break;

        case SIGBUS:
            log(Log::INFO, "", "Received SIGBUS (Bus error: invalid address alignment or non-existent physical memory). Server terminated.");
            break;

        case SIGPIPE:
            log(Log::INFO, "", "Received SIGPIPE (Write to a pipe or socket with no reader). Server terminated.");
            break;

        case SIGHUP:
            log(Log::INFO, "", "Received SIGHUP (Terminal closed or controlling process terminated). Server terminated.");
            break;

        case SIGALRM:
            log(Log::INFO, "", "Received SIGALRM (Timer signal from alarm()). Server terminated.");
            break;

        case SIGQUIT:
            log(Log::INFO, "", "Received SIGQUIT (Quit from keyboard: Ctrl+\\). Server terminated.");
            break;

        default:
            log(Log::INFO, "", "Received unknown signal (" + std::to_string(signum) + "). Server terminated.");
            break;
    }
    exit(signum);
}
