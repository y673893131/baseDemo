#include "tableviewdelagate.h"
#include <QCheckBox>
#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include "tableviewmodel.h"
TableViewDelagate::TableViewDelagate(QObject* parent)
    :QStyledItemDelegate(parent)
{

}


void TableViewDelagate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto type = index.data(TableViewModel::UserRole_Type).toInt();
    switch (type) {
    case Type_Button:
        paintButton(painter, option, index);
        break;
    case Type_HyperLink:
        paintHyperLink(painter, option, index);
        break;
    case Type_Progress:
        paintProgress(painter, option, index);
        break;
    default:
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void TableViewDelagate::paintButton(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    if(option.state.testFlag(QStyle::State_MouseOver))
        painter->setPen("white");
    else
        painter->setPen("lightgray");
    painter->fillRect(option.rect, QColor("#2E2F30"));
    painter->drawText(option.rect, Qt::AlignCenter, index.data().toString());
    painter->restore();
}

void TableViewDelagate::paintHyperLink(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 viewOption(option);
    initStyleOption(&viewOption, index);
    painter->save();
    if(option.state.testFlag(QStyle::State_MouseOver))
    {
        viewOption.palette.setColor(QPalette::All, QPalette::Text, "blue");
        viewOption.palette.setColor(QPalette::Inactive, QPalette::HighlightedText, "blue");
    }
    else
    {
        viewOption.palette.setColor(QPalette::All, QPalette::Text, "#0080c0");
        viewOption.palette.setColor(QPalette::Inactive, QPalette::HighlightedText, "#0080c0");
    }
    viewOption.font.setUnderline(true);
    QStyledItemDelegate::paint(painter, viewOption, index);
    painter->restore();
}

void TableViewDelagate::paintProgress(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    auto rc = option.rect;
    auto min = index.data(TableViewModel::UserRole_Range_Min).toInt();
    auto max = index.data(TableViewModel::UserRole_Range_Max).toInt();
    auto value = index.data(TableViewModel::UserRole_Range_Value).toInt();

    auto percent = 1.0 * (value - min) / (max - min);
    percent = qMin(1.0, percent);
    percent = qMax(0.0, percent);
    QColor color("red");
    if(percent > 0.8)
        color = "#41CD52";
    else if(percent > 0.3)
        color = "#FECD44";
    painter->fillRect(QRect(rc.x(), rc.y(), percent * rc.width(), rc.height()), color);
    painter->drawText(option.rect, Qt::AlignCenter, index.data(TableViewModel::UserRole_Range_Value).toString());

    painter->restore();
}

QWidget *TableViewDelagate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto type = index.data(TableViewModel::UserRole_Type).toInt();
    switch (type){
    case Type_CheckBox:
    {
        QCheckBox* checkbox = new QCheckBox(parent);
        checkbox->setText(index.data().toString());
        checkbox->setStyleSheet(QString("QCheckBox{background: white;padding-left: 5px;}"));
        checkbox->setBackgroundRole(parent->backgroundRole());
        QTimer::singleShot(0, checkbox, SIGNAL(clicked()));
        return checkbox;
    }
    case Type_Combobox:
    {
        auto combobox = new QComboBox(parent);
        combobox->addItems({"combobox:1","combobox:2","combobox:3","combobox:4"});
        return combobox;
    }
    case Type_Button:
    {
        QPushButton* button = new QPushButton(parent);
        button->setText(index.data().toString());
        button->setStyleSheet("QPushButton{background: #2E2F30;color: white;}QPushButton::pressed{background: #212222;color: #AFAFAF;}");
        connect(button, &QPushButton::clicked, [button]{
            QMessageBox::information(button, "提示", button->text());
        });

        QTimer::singleShot(0, button, SIGNAL(clicked()));
        return button;
    }
    case Type_HyperLink:
    {
        QPushButton* button = new QPushButton(parent);
        button->setStyleSheet("QPushButton{background: white; margin:0px;border: none; color: #0080c0;text-decoration: underline;}QPushButton::hover{color: blue;}");
        connect(button, &QPushButton::clicked, [button, index]{
            QMessageBox::information(button, "提示", index.data(TableViewModel::UserRole_HyperLink_Value).toString());
        });

        QTimer::singleShot(0, button, SIGNAL(clicked()));
        return button;
    }
    case Type_Edit:
        return new QTextEdit(parent);
    case Type_LineEdit:
        return QStyledItemDelegate::createEditor(parent, option, index);
    case Type_SpinBox:
    {
        auto spinbox = new QSpinBox(parent);
        connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(commit()));
        return spinbox;
    }
    case Type_Slider:
    {
        auto slider = new QSlider(Qt::Horizontal, parent);
        slider->setStyleSheet("QSlider{background: white;}");
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(commit()));
        return slider;
    }
    default:
        return nullptr;
    }
}

void TableViewDelagate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto type = index.data(TableViewModel::UserRole_Type).toInt();
    switch (type){
    case Type_CheckBox:
    {
        auto checkbox = reinterpret_cast<QCheckBox*>(editor);
        checkbox->setCheckState(index.data(Qt::CheckStateRole).value<Qt::CheckState>());
        break;
    }
    case Type_Combobox:
    {
        editor->setProperty("currentText", index.data());
        break;
    }
    case Type_LineEdit:
    case Type_HyperLink:
    {
        editor->setProperty("text", index.data());
        break;
    }
    case Type_Edit:
    {
        editor->setProperty("plainText", index.data());
        reinterpret_cast<QTextEdit*>(editor)->selectAll();
        break;
    }
    case Type_SpinBox:
    case Type_Slider:
    {
        editor->setProperty("minimum", index.data(TableViewModel::UserRole_Range_Min));
        editor->setProperty("maximum", index.data(TableViewModel::UserRole_Range_Max));
        editor->setProperty("value", index.data(TableViewModel::UserRole_Range_Value));
        break;
    }
    }
}

void TableViewDelagate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto type = index.data(TableViewModel::UserRole_Type).toInt();
    if(type == Type_Edit)
        editor->setGeometry(QRect(option.rect.topLeft(), option.rect.size() * 2));
    else
        editor->setGeometry(option.rect);
}

void TableViewDelagate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto type = index.data(TableViewModel::UserRole_Type).toInt();
    switch (type){
    case Type_CheckBox:
    {
        auto checkbox = reinterpret_cast<QCheckBox*>(editor);
        model->setData(index, checkbox->checkState(), Qt::CheckStateRole);
        break;
    }
    case Type_LineEdit:
    {
        model->setData(index, editor->property("text"), Qt::DisplayRole);
        break;
    }
    case Type_Edit:
    {
        model->setData(index, editor->property("plainText"), Qt::DisplayRole);
        break;
    }
    case Type_Combobox:
    {
        model->setData(index, editor->property("currentText"), Qt::DisplayRole);
        break;
    }
    case Type_SpinBox:
    case Type_Slider:
    {
        model->setData(index, editor->property("value").toInt(), TableViewModel::UserRole_Range_Value);
        break;
    }
    }
}

void TableViewDelagate::commit()
{
    auto editor = reinterpret_cast<QWidget*>(sender());
    commitData(editor);
}
