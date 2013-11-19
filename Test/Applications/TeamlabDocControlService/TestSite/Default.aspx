<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Document server control page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <strong><span style="font-size: 16pt">Document server control page</span></strong><table>
            <caption>
            </caption>
            <tr>
                <td style="width: 100px">
                    Name</td>
                <td style="width: 100px">
                    Action</td>
                <td style="width: 100px">
                    Result</td>
            </tr>
            <tr>
                <td style="width: 100px; height: 26px">
                    Check status</td>
                <td style="width: 100px; height: 26px">
                    <asp:Button ID="btnStatusCheck" runat="server" OnClick="btnStatusCheck_Click" Text="Status " /></td>
                <td style="width: 100px; height: 26px">
                    <asp:Label ID="lblStatusResult" runat="server" Text="Unknown"></asp:Label></td>
            </tr>
            <tr>
                <td style="width: 100px">
                    Processor usage</td>
                <td style="width: 100px">
                    <asp:Button ID="btnProcessorUsage" runat="server" OnClick="btnProcessorUsage_Click"
                        Text="Check" /></td>
                <td style="width: 100px">
                    <asp:Label ID="lblProcessorUsage" runat="server" Text="N/A"></asp:Label></td>
            </tr>
            <tr>
                <td style="width: 100px">
                    Memory usage</td>
                <td style="width: 100px">
                    <asp:Button ID="btnMemoryUsage" runat="server" Text="Check" /></td>
                <td style="width: 100px">
                    <asp:Label ID="lblMemoryUsage" runat="server" Text="N/A"></asp:Label></td>
            </tr>
            <tr>
                <td style="width: 100px">
                    Url checker</td>
                <td style="width: 100px">
                    <asp:Button ID="btnUrlChecker" runat="server" Text="Check" /></td>
                <td style="width: 100px">
                    <asp:Label ID="lblUrlChecker" runat="server" Text="N/A"></asp:Label></td>
            </tr>
            <tr>
                <td style="width: 100px">
                    Clear folder</td>
                <td style="width: 100px">
                    <asp:Button ID="btnClearFolder" runat="server" Text="Clear" /></td>
                <td style="width: 100px">
                    <asp:Label ID="lblClearFolder" runat="server" Text="N/A"></asp:Label></td>
            </tr>
            <tr>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
            </tr>
            <tr>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
            </tr>
            <tr>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
            </tr>
            <tr>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
                <td style="width: 100px">
                </td>
            </tr>
        </table>
    
    </div>
    </form>
</body>
</html>
