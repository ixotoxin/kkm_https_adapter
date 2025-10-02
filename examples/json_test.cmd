@ECHO OFF
CLS
%~dp0..\_build\kkmha.exe exec + %~dp0json\base-status.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\status.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\full-status.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\cash-in.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\cash-out.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-demo.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-fn-registrations.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-info.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-non-fiscal-doc.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-ofd-exchange-status.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-ofd-test.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\sell.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\sell-return.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-last-document.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\report-x.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\close-shift.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\cash-stat.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\print-close-shift-reports.json
%~dp0..\_build\kkmha.exe exec + %~dp0json\reset-state.json
