#include <QDateTime>
#include <QLabel>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>
#include <utility>

#include "globals.h"
#include "gui/SolvedGUI.h"

namespace sudoku
{
    SolvedGUI::SolvedGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, std::string  name, QSettings* settings, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          _name(std::move(name)),
          _gridWidget(new QWidget(this, Qt::WindowFlags())),
          _settings(settings),
          _exportPngButton(new QPushButton(this)),
          _exportPdfButton(new QPushButton(this))
    {
        constexpr QSize guiDim(537, 587);
        this->setFixedSize(guiDim);
        this->setObjectName(QStringLiteral("SolvedGUI"));
        this->setWindowTitle(QStringLiteral("Solved Sudoku"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        _gridWidget->setObjectName(QStringLiteral("gridWidget"));
        constexpr QRect gridWidgetGeom(0, 0, 537, 537);
        _gridWidget->setGeometry(gridWidgetGeom);
        _gridWidget->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        const QFont buttonFont(QStringLiteral("Open Sans"), 10, QFont::Bold, false);
        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        _exportPngButton->setObjectName(QStringLiteral("exportPngButton"));
        constexpr QRect pngButtonGeom(0, 537, 268, 50);
        _exportPngButton->setGeometry(pngButtonGeom);
        _exportPngButton->setFont(buttonFont);
        _exportPngButton->setStyleSheet(buttonStyleSheet);
        _exportPngButton->setText(QStringLiteral("Export PN&G"));
        SolvedGUI::connect(_exportPngButton, &QPushButton::clicked, this, [this]()
                           { this->exportPNG(_gridWidget); }, Qt::AutoConnection);

        _exportPdfButton->setObjectName(QStringLiteral("exportPdfButton"));
        constexpr QRect pdfButtonGeom(269, 537, 268, 50);
        _exportPdfButton->setGeometry(pdfButtonGeom);
        _exportPdfButton->setFont(buttonFont);
        _exportPdfButton->setStyleSheet(buttonStyleSheet);
        _exportPdfButton->setText(QStringLiteral("Export PD&F"));
        SolvedGUI::connect(_exportPdfButton, &QPushButton::clicked, this, [this]()
                           { this->exportPDF(_gridWidget); }, Qt::AutoConnection);

        SolvedGUI::drawFields(sudoku->getSteps()->back(), sudoku->getSteps()->back(), initVals, _gridWidget);
        SolvedGUI::drawFrame(_gridWidget);
    }

    void SolvedGUI::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }

    auto SolvedGUI::renderPixmap(QWidget* parent) -> QPixmap
    {
        // Generate pixmap with solved sudoku
        auto dim = 2 * parent->size();
        QPixmap pixmap(dim);
        pixmap.setDevicePixelRatio(2);
        parent->render(&pixmap, QPoint(), QRegion(), QWidget::DrawWindowBackground | QWidget::DrawChildren);
        return pixmap;
    }

    auto SolvedGUI::exportPNG(QWidget* parent) -> void
    {
        auto pixmap = SolvedGUI::renderPixmap(parent);
        if (_name.empty())
        {
            _name = "sudoku";
        }
        pixmap.save(_settings->value(QStringLiteral("DIRS/DataDir"), QVariant()).toString() + QStringLiteral("/export/") + QString::fromStdString(_name) + QStringLiteral("-solution-") + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + QStringLiteral(".png"), "png", 0);
    }

    auto SolvedGUI::exportPDF(QWidget* parent) -> void
    {
        auto pixmap = SolvedGUI::renderPixmap(parent);
        if (_name.empty())
        {
            _name = "sudoku";
        }

        // Create output pdf
        QPdfWriter pdfWriter(_settings->value(QStringLiteral("DIRS/DataDir"), QVariant()).toString() + QStringLiteral("/export/") + QString::fromStdString(_name) + QStringLiteral("-solution-") + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + QStringLiteral(".pdf"));
        pdfWriter.setPageSize(QPageSize(2 * parent->size(), QPageSize::Point, QString(), QPageSize::FuzzyMatch));
        pdfWriter.setPageMargins(QMarginsF(0, 0, 0, 0));

        // Draw pixmap on pdf
        QPainter painter(&pdfWriter);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.setRenderHint(QPainter::LosslessImageRendering, true);
        painter.drawPixmap(QRect(0, 0, pdfWriter.width(), pdfWriter.height()), pixmap);
        painter.end();
    }

    // Helper functions
    void SolvedGUI::drawFields(Step& currStep, Step& nextStep, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
    {
#pragma unroll static_cast<short>(global::order * global::order * global::order)
        for (QLabel* label : parent->findChildren<QLabel*>(QString(), Qt::FindChildrenRecursively)) {
            label->deleteLater();
        }
        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 28, QFont::Bold, false);
        const QFont candsFont(QStringLiteral("Liberation Mono"), 12, QFont::Bold, false);
        const QFont rcLabelFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);

        uint8_t fID = 1;
        constexpr uint8_t offset = 27;
        uint16_t posY = offset;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            auto* rLabel = std::make_unique<QLabel>(parent, Qt::WindowFlags()).release();
            rLabel->setObjectName("rLabel" + QString::number(rID, global::base));
            const QRect rLabelGeom(posY, 1, global::fieldDim, 25);
            rLabel->setGeometry(rLabelGeom);
            rLabel->setStyleSheet(QStringLiteral("color: rgb(100, 100, 100); background-color: rgba(239, 239, 239, 1.0); border: 1px solid rgb(171, 171, 171)"));
            rLabel->setFont(rcLabelFont);
            rLabel->setAlignment(Qt::AlignCenter);
            rLabel->setText(QString::number(rID, global::base));
            rLabel->show();

            auto* cLabel = std::make_unique<QLabel>(parent, Qt::WindowFlags()).release();
            cLabel->setObjectName("rLabel" + QString::number(rID, global::base));
            const QRect cLabelGeom(1, posY, 25, global::fieldDim);
            cLabel->setGeometry(cLabelGeom);
            cLabel->setStyleSheet(QStringLiteral("color: rgb(100, 100, 100); background-color: rgba(239, 239, 239, 1.0); border: 1px solid rgb(171, 171, 171)"));
            cLabel->setFont(rcLabelFont);
            cLabel->setAlignment(Qt::AlignCenter);
            cLabel->setText(QString::number(rID, global::base));
            cLabel->show();

            uint16_t posX = offset;
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                auto* fieldLabel = std::make_unique<QLabel>(parent, Qt::WindowFlags()).release();
                fieldLabel->show();
                fieldLabel->setObjectName("fieldLabel" + QString::number(fID, global::base));
                const QRect fieldGeom(posX, posY, global::fieldDim, global::fieldDim);
                fieldLabel->setGeometry(fieldGeom);
                fieldLabel->setFont(fieldsFont);
                fieldLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
                fieldLabel->setAlignment(Qt::AlignCenter);
                fieldLabel->setFrameShape(QFrame::Panel);
                const uint8_t val = *nextStep.getGrid()->at(rID - 1).at(cID - 1).getVal();
                // If fieldLabel is solved => fill value
                if (val != 0 && val == *currStep.getGrid()->at(rID - 1).at(cID - 1).getVal())
                {
                    fieldLabel->setText(QString::number(val, global::base));
                    // If fieldLabel was NOT given initially, but is solved now => set color green
                    if (initVals.at(fID - 1) == 0)
                    {
                        fieldLabel->setStyleSheet(QStringLiteral("color: rgb(20,160,50); background-color: rgba(239, 239, 239, 1.0); border: 1px solid black"));
                    }
                }
                else
                {
                    uint16_t candY = posY + 1;
                    uint8_t candI = 1;
                    for (uint8_t candR = 1; candR <= 3; candR++)
                    {
                        uint16_t candX = posX + 1;
                        for (uint8_t candC = 1; candC <= 3; candC++)
                        {
                            auto* cand = std::make_unique<QLabel>(parent, Qt::WindowFlags()).release();
                            const QRect candGeom(candX, candY, global::candDim, global::candDim);
                            cand->setGeometry(candGeom);
                            cand->setAlignment(Qt::AlignCenter);
                            cand->setFont(candsFont);
                            cand->setStyleSheet(QStringLiteral("color: rgb(20,50,255)"));
                            auto* cands = currStep.getGrid()->at(rID - 1).at(cID - 1).getCandidates();
                            if (std::find(cands->begin(), cands->end(), candI) != cands->end())
                            {
                                cand->setText(QString::number(candI, global::base));
                                if (&currStep != &nextStep) // enable this: hide green markings after step execution.
                                {
                                    // If candI disappears in the next step => make background of cand red
                                    if (std::find(nextStep.getGrid()->at(rID - 1).at(cID - 1).getCandidates()->begin(), nextStep.getGrid()->at(rID - 1).at(cID - 1).getCandidates()->end(), candI) == nextStep.getGrid()->at(rID - 1).at(cID - 1).getCandidates()->end())
                                    {
                                        cand->setStyleSheet(QStringLiteral("color: rgb(10,30,255); background-color: rgba(255, 70, 0, 1.0)"));
                                    }
                                    // set green background for cands found in "fields" in next step
                                    for (const Field* field : *nextStep.getFields()) // go through all fields in nextStep, where candidates were found
                                    {
                                        const std::vector<uint8_t> fieldCands = *nextStep.getCandidates(); // get the candidates found in the fieldLabel
#pragma unroll static_cast<short>(global::order)
                                        for (const uint8_t fieldCand : fieldCands)
                                        {
                                            if (*field->getFID() == *nextStep.getGrid()->at(rID - 1).at(cID - 1).getFID() && candI == fieldCand)
                                            {
                                                cand->setStyleSheet(QStringLiteral("color: rgb(10,30,255); background-color: rgba(50, 255, 10, 1.0)"));
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                cand->setText(QLatin1String(""));
                            }
                            if (candI == *nextStep.getGrid()->at(rID - 1).at(cID - 1).getVal()) // set green candidate background for fields solved in next step
                            {
                                cand->setStyleSheet(QStringLiteral("color: rgb(10,30,255); background-color: rgba(50, 255, 10, 1.0)"));
                            }
                            cand->show();
                            candI++;
                            candX += global::candDim;
                        }
                        candY += global::candDim;
                    }
                }
                fID++;
                posX += global::fieldDim;
                if (cID % 3 == 0)
                {
                    posX += 2;
                }
            }
            posY += global::fieldDim;
            if (rID % 3 == 0)
            {
                posY += 2;
            }
        }
    }

    void SolvedGUI::drawFrame(QWidget* parent)
    {
        // Frame for the grid
        auto* hLine_1 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        hLine_1->setObjectName(QStringLiteral("_hLine0"));
        constexpr QRect hLine_1Geom(25, 0, 512, 2);
        hLine_1->setGeometry(hLine_1Geom);
        hLine_1->setLineWidth(2);
        hLine_1->setFrameShape(QFrame::HLine);
        hLine_1->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine0 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        hLine0->setObjectName(QStringLiteral("_hLine0"));
        constexpr QRect hLine0Geom(0, 25, 537, 2);
        hLine0->setGeometry(hLine0Geom);
        hLine0->setLineWidth(2);
        hLine0->setFrameShape(QFrame::HLine);
        hLine0->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine1 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        hLine1->setObjectName(QStringLiteral("_hLine1"));
        constexpr QRect hLine1Geom(0, 195, 537, 2);
        hLine1->setGeometry(hLine1Geom);
        hLine1->setLineWidth(2);
        hLine1->setFrameShape(QFrame::HLine);
        hLine1->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine2 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        hLine2->setObjectName(QStringLiteral("_hLine2"));
        constexpr QRect hLine2Geom(0, 365, 537, 2);
        hLine2->setGeometry(hLine2Geom);
        hLine2->setLineWidth(2);
        hLine2->setFrameShape(QFrame::HLine);
        hLine2->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine3 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        hLine3->setObjectName(QStringLiteral("_hLine3"));
        constexpr QRect hLine3Geom(0, 535, 537, 2);
        hLine3->setGeometry(hLine3Geom);
        hLine3->setLineWidth(2);
        hLine3->setFrameShape(QFrame::HLine);
        hLine3->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine_1 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        vLine_1->setObjectName(QStringLiteral("vLine0"));
        constexpr QRect vLine_1Geom(0, 25, 2, 512);
        vLine_1->setGeometry(vLine_1Geom);
        vLine_1->setLineWidth(2);
        vLine_1->setFrameShape(QFrame::VLine);
        vLine_1->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine0 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        vLine0->setObjectName(QStringLiteral("vLine0"));
        constexpr QRect vLine0Geom(25, 0, 2, 537);
        vLine0->setGeometry(vLine0Geom);
        vLine0->setLineWidth(2);
        vLine0->setFrameShape(QFrame::VLine);
        vLine0->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine1 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        vLine1->setObjectName(QStringLiteral("vLine1"));
        constexpr QRect vLine1Geom(195, 0, 2, 537);
        vLine1->setGeometry(vLine1Geom);
        vLine1->setLineWidth(2);
        vLine1->setFrameShape(QFrame::VLine);
        vLine1->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine2 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        vLine2->setObjectName(QStringLiteral("vLine2"));
        constexpr QRect vLine2Geom(365, 0, 2, 537);
        vLine2->setGeometry(vLine2Geom);
        vLine2->setLineWidth(2);
        vLine2->setFrameShape(QFrame::VLine);
        vLine2->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine3 = std::make_unique<QFrame>(parent, Qt::WindowFlags()).release();
        vLine3->setObjectName(QStringLiteral("vLine3"));
        constexpr QRect vLine3Geom(535, 0, 2, 537);
        vLine3->setGeometry(vLine3Geom);
        vLine3->setLineWidth(2);
        vLine3->setFrameShape(QFrame::VLine);
        vLine3->setStyleSheet(QStringLiteral("color: black"));
    }
} // namespace sudoku
