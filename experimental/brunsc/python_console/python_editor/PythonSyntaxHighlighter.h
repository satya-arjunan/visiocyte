#ifndef VISIOCYTE_PYTHON_SYNTAX_HIGHLIGHTER_H_
#define VISIOCYTE_PYTHON_SYNTAX_HIGHLIGHTER_H_

#include <QSyntaxHighlighter>

class PythonSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    enum Format {
        Other,
        Keyword,
        String,
        Comment,
        FinalFormat = Comment
    };

    PythonSyntaxHighlighter(QTextDocument* parent);
    virtual void highlightBlock(const QString& text_block);

protected:
    enum State {
        NormalState = -1,
        InComment,
        InString
    };

private:
    QTextCharFormat m_format[FinalFormat + 1];
};

#endif /* VISIOCYTE_PYTHON_SYNTAX_HIGHLIGHTER_H_ */
