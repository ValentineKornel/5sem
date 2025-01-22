document.getElementById('buttonJson').addEventListener('click', function(event) {
    event.preventDefault();

    fetch('http://localhost:5000/json/file2.json', {
        method: 'GET',
    })
    .then(response => {
        if (response.ok) {
            return response.json();
        } else {
            throw new Error('Failed to fetch the file');
        }
    })
    .then(blob => {
        const jsonContent = document.getElementById('json');
        jsonContent.textContent = JSON.stringify(blob, 0, 2);
    })
    .catch(error => {
        console.error('Error fetching file:', error);
    });
});

document.getElementById('buttonXml').addEventListener('click', function(event) {
    event.preventDefault();

    fetch('http://localhost:5000/xml/file3.xml', {
        method: 'GET',
    })
    .then(response => {
        if (response.ok) {
            return response.text();
        } else {
            throw new Error('Failed to fetch the file');
        }
    })
    .then(xmlString => {
    const parser = new DOMParser();
    const xmlDoc = parser.parseFromString(xmlString, "text/xml");
    const movies = xmlDoc.getElementsByTagName('movie');
    let output = '';

    for (let i = 0; i < movies.length; i++) {
        const title = movies[i].getElementsByTagName('title')[0].textContent;
        const author = movies[i].getElementsByTagName('director')[0].textContent;
        const year = movies[i].getElementsByTagName('year')[0].textContent;
        const genre = movies[i].getElementsByTagName('genre')[0].textContent;


        output += `${title} by ${author} (${year}) genre: ${genre}<br>`;
    }
    //output = xmlString;

    const resultContainer = document.getElementById('xml');
    resultContainer.innerHTML = output;
})
})
