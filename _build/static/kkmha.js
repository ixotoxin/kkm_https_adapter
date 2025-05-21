/* TODO: Реализовать базовую функциональность для взаимодействия с KKMHA API. */

function generateIdempotencyKey() {
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    let result = '';
    for (let i = 0; i < 32; i++) {
        const randomIndex = Math.floor(Math.random() * characters.length);
        result += characters.charAt(randomIndex);
    }
    return result;
}

function responseLogger(xhr) {
    console.log("Response status: ", xhr.statusText);
    if (xhr.status === 200) {
        console.log(xhr.responseText);
    }
}

function performGet(method, callback) {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", method);
    xhr.onreadystatechange = () => {
        if (xhr.readyState === 4) {
            callback(xhr);
        }
    };
    xhr.send();
}

function performPost(method, data, callback) {
    const xhr = new XMLHttpRequest();
    xhr.open("POST", method);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.setRequestHeader("X-Idempotency-Key", generateIdempotencyKey());
    xhr.onreadystatechange = () => {
        callback(xhr);
    };
    xhr.send(JSON.stringify(data));
}
