// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "AssetDataList.h"

#include "AssetData.h"
#include "AssetRegistryModule.h"
#include "Editor.h"

#include "DragAndDrop/AssetDragDropOp.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"

// Only required for the LogCesiumEditor definition:
#include "CesiumEditor.h"

AssetDataList::AssetDataList() {

  // Obtain the asset registry and register the callback to
  // call `_HandleFilesLoaded` when all assets are loaded.
  FAssetRegistryModule& assetRegistryModule =
      FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
  assetRegistryModule.Get().OnFilesLoaded().AddRaw(
      this,
      &AssetDataList::_HandleFilesLoaded);
}

AssetDataList::~AssetDataList() {
  // Unregister the callback that was attached in the constructor.
  FAssetRegistryModule& AssetRegistryModule =
      FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
  AssetRegistryModule.Get().OnFilesLoaded().RemoveAll(this);
}

void AssetDataList::_HandleFilesLoaded() {
  UE_LOG(
      LogCesiumEditor,
      Verbose,
      TEXT("Asset registry finished loading, creating assets..."));
  for (const auto& objectPath : _pendingObjectPaths) {
    _AddAssetInternal(objectPath);
  }
  _pendingObjectPaths.clear();
}

void AssetDataList::Construct(const FArguments& Args) {

  TSharedPtr<SHeaderRow> headerRow =
      SNew(SHeaderRow) +
      SHeaderRow::Column("MainColumn").DefaultLabel(Args._Title);

  this->_listView = SNew(SListView<TSharedPtr<FAssetData>>)
                        .ItemHeight(24)
                        .ListItemsSource(&_items)
                        .OnGenerateRow(this, &AssetDataList::_CreateRow)
                        .HeaderRow(headerRow);
  ChildSlot
      [SNew(SScrollBox) + SScrollBox::Slot()[this->_listView.ToSharedRef()]];
}

void AssetDataList::AddAsset(const std::string& objectPath) {

  FAssetRegistryModule& AssetRegistryModule =
      FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
  if (AssetRegistryModule.Get().IsLoadingAssets()) {
    FString objectPathString(objectPath.c_str());
    UE_LOG(
        LogCesiumEditor,
        Verbose,
        TEXT("Adding pending asset from object path %s"),
        *objectPathString);

    _pendingObjectPaths.push_back(objectPath);
  } else {
    _AddAssetInternal(objectPath);
  }
}

void AssetDataList::_AddAssetInternal(const std::string& objectPath) {
  FString objectPathString(objectPath.c_str());

  FAssetRegistryModule& AssetRegistryModule =
      FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
  IAssetRegistry& assetRegistry = AssetRegistryModule.Get();
  if (assetRegistry.IsLoadingAssets()) {
    UE_LOG(
        LogCesiumEditor,
        Warning,
        TEXT(
            "Adding asset from object path %s is not possible while loading assets"),
        *objectPathString);
    return;
  }

  UE_LOG(
      LogCesiumEditor,
      Verbose,
      TEXT("Adding asset from object path %s"),
      *objectPathString);

  FAssetData assetData =
      assetRegistry.GetAssetByObjectPath(FName(*objectPathString));

  UE_LOG(
      LogCesiumEditor,
      Verbose,
      TEXT("Adding asset from asset data %s"),
      *assetData.GetFullName());

  UObject* asset = assetData.GetAsset();
  if (asset) {
    this->_items.Add(MakeShared<FAssetData>(assetData));
    this->_listView->RequestListRefresh();

  } else {
    UE_LOG(
        LogCesiumEditor,
        Warning,
        TEXT("Could not add asset from object path %s"),
        *objectPathString);
  }
}

namespace {

/**
 * Tries to obtain the tooltip text for the given asset data.
 *
 * If no suitable tooltip text can be obtained, then the full
 * name of the asset data will be returned.
 *
 * @param assetData The asset data
 * @return The tooltip text
 */
FText obtainToolTipText(TSharedPtr<FAssetData> assetData) {

  FString blueprintDescription = assetData->GetTagValueRef<FString>(
      GET_MEMBER_NAME_CHECKED(UBlueprint, BlueprintDescription));
  if (!blueprintDescription.IsEmpty()) {
    return FText::FromString(blueprintDescription);
  }
  UClass* cls =
      FindObject<UClass>(ANY_PACKAGE, *assetData->ObjectPath.ToString());
  if (!cls) {
    return FText::FromString(assetData->GetFullName());
  }
  return cls->GetToolTipText();
}

/**
 * @brief An internal class representing a single row in the asset list.
 *
 * It creates a visual representation of the `FAssetData` that was
 * found in the `_items` of the `AssetDataList`.
 */
class FAssetDataRow : public STableRow<TSharedPtr<FAssetData>> {
public:
  void Construct(
      const FArguments& InArgs,
      const TSharedRef<STableViewBase>& InOwnerTableView,
      const TSharedPtr<FAssetData>& pItem) {
    STableRow<TSharedPtr<FAssetData>>::Construct(InArgs, InOwnerTableView);
    TSharedRef<SOverlay> overlay = SNew(SOverlay);

    // A single slot that only shows the class name
    TSharedPtr<STextBlock> assetNameText =
        SNew(STextBlock)
            .AutoWrapText(true)
            .Text(FText::FromString(pItem->AssetName.ToString()));

    overlay->AddSlot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Center)
        .Padding(5.0f, 5.0f)[assetNameText.ToSharedRef()];

    // The tooltip for the row.
    FText toolTipText = obtainToolTipText(pItem);
    overlay->SetToolTip(SNew(SToolTip)[SNew(STextBlock).Text(toolTipText)]);
    ChildSlot[overlay];
  }
};

} // namespace

TSharedRef<ITableRow> AssetDataList::_CreateRow(
    TSharedPtr<FAssetData> item,
    const TSharedRef<STableViewBase>& list) {
  return SNew(FAssetDataRow, list, item)
      .OnDragDetected(this, &AssetDataList::_OnDragging);
}

FReply AssetDataList::_OnDragging(
    const FGeometry& MyGeometry,
    const FPointerEvent& MouseEvent) {
  UE_LOG(LogCesiumEditor, Verbose, TEXT("Starting drag-and-drop..."));

  if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
    TArray<TSharedPtr<FAssetData>> SelectedItems =
        _listView->GetSelectedItems();
    if (SelectedItems.Num() == 1) {
      TArray<FAssetData> InAssetData;
      FAssetData assetData = *SelectedItems[0];

      UE_LOG(
          LogCesiumEditor,
          Verbose,
          TEXT("Dragging asset data %s"),
          *assetData.GetFullName());

      InAssetData.Add(assetData);
      return FReply::Handled().BeginDragDrop(
          FAssetDragDropOp::New(InAssetData));
    }
  }
  return FReply::Unhandled();
}
