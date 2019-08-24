#ifndef COLORSMODEL_H
#define COLORSMODEL_H

#include <QObject>
#include <memory>
#include <backend/dominantcolor/DominantColor.h>
#include <map>

class ColorsModel : public QObject {
    class ColorsModelImpl;
    Q_OBJECT
public:
    explicit ColorsModel(QObject *parent = nullptr);
    ~ColorsModel();

    // To mark what changed: nothing, new colors added, changed background color
    enum class ChangedState { None, NewColors, Background };

    // Storing colors data
    struct Colors {
        Colors();

        ChangedState changedState_;

        std::vector<color> BGFG_;        // Background and Foreground colors
        std::vector<color> BGFGintense_; // Same as above, but intense
        std::vector<color> regular_;     // Regular colors
        std::vector<color> intense_;     // Intense colors
    };

    // Methods for Colors
    const Colors &getColors() const;
    void setImgColors(const std::vector<color> &colors);
    void setBGFGColors(const std::vector<color> &bgfg);

public slots:
    void onImageDropped(const QString& path);

signals:
    void hideImageDropWidget();
    void doProcessColors(const std::string &);
    void modelChanged();

private:
    std::unique_ptr<ColorsModelImpl> pimpl_;
};

#endif // COLORSMODEL_H
