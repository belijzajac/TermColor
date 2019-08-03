#include "GuiModel.h"

class GuiModel::GuiModelImpl {
public:
    explicit GuiModelImpl(GuiModel &p);

private:
    GuiModel &parent_;
};

GuiModel::GuiModelImpl::GuiModelImpl(GuiModel &p) : parent_{p} {

}

GuiModel::GuiModel(QObject *parent) : QObject{parent} {
    pimpl_ = std::make_unique<GuiModelImpl>(*this);
}

GuiModel::~GuiModel() = default;
