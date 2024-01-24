#include "Threading.h"
#include "Nists.h"

Threading::Threading(MainWindow *mainWindow, const bool TestsToRun[AmountOfNists]) {
  for (uint8_t i = 0; i < AmountOfNists; i++) {
    this->TestToRun[i] = TestsToRun[i] ? true : false;
  }
  this->mainWindow = mainWindow;
}

void Threading::run() {
  uint8_t results[AmountOfNists];

  results[0] = this->TestToRun[0] ? FrequencyMonobitTest(this->mainWindow->GeneratedSelection,
                                                         this->mainWindow->GeneratedSelectionSize)
                                  : 2;
  results[1] = this->TestToRun[1] ? FrequencyBlockTest(this->mainWindow->GeneratedSelection,
                                                       this->mainWindow->GeneratedSelectionSize,
                                                       this->mainWindow->AmountOfBlocks->value())
                                  : 2;
  results[2] = this->TestToRun[2] ? TestForSequenceOfIdenticalBits(this->mainWindow->GeneratedSelection,
                                                                   this->mainWindow->GeneratedSelectionSize)
                                  : 2;
  results[3] = this->TestToRun[3] ? TestForLongestRunOfOnesInABlock(this->mainWindow->GeneratedSelection,
                                                                    this->mainWindow->GeneratedSelectionSize,
                                                                    this->mainWindow->AmountOfBlocks->value())
                                  : 2;
  results[4] = this->TestToRun[4] ? BinaryMatrixRankTest(this->mainWindow->GeneratedSelection,
                                                         this->mainWindow->GeneratedSelectionSize)
                                  : 2;
  results[5] = this->TestToRun[5] ? SpectrumTest(this->mainWindow->GeneratedSelection,
                                                 this->mainWindow->GeneratedSelectionSize)
                                  : 2;
  results[6] = this->TestToRun[6]
                   ? NonOverlappingTemplateMatchingTest(this->mainWindow->GeneratedSelection,
                                                        this->mainWindow->GeneratedSelectionSize,
                                                        this->mainWindow->FilePath)
                   : 2;
  results[7] = this->TestToRun[7] ? OverlappingTemplateMatchingTest(this->mainWindow->GeneratedSelection,
                                                                    this->mainWindow->GeneratedSelectionSize,
                                                                    this->mainWindow->FilePath)
                                  : 2;

  memcpy(&this->mainWindow->ThreadBuffer, &results, sizeof(this->mainWindow->ThreadBuffer));
}