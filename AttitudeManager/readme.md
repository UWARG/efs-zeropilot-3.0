  Welcome to Attitude Manager!

```mermaid
%%{
  init: {
    "theme": "base",
    "fontFamily": "monospace",
    "logLevel": "info",
    "flowchart": {
      "htmlLabels": true
    }
  }
}%%
graph TD;
  Init--Init Error-->Init
  Init--Init OK-->FetchInstructionsMode
  
  FetchInstructionsMode--Disarmed-->DisarmMode
  FetchInstructionsMode--Comms Timeout-->FatalFailureMode
  FetchInstructionsMode--Armed-->GetPositionMode
  
  DisarmMode--Armed-->FetchInstructionsMode

  GetPositionMode-->RunControlsMode
  
  RunControlsMode-->OutputMixingMode
  
  OutputMixingMode--Success-->StateReportingMode
  OutputMixingMode--Error-->FatalFailureMode
  
  StateReportingMode-->FetchInstructionsMode
```