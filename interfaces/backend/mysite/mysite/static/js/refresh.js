$(document).ready(function(){
    refresh();
});

function refresh()
{
    $.ajax({
        url: "/Dashboard/ReloadData/",
        success: function(data)
		{
            $('#DashboardTable').html(data);
        }
    });
    setInterval("refresh()", 1000);
}
